#include "vision/Detector.h"
#include <opencv2/imgproc.hpp>
#include <algorithm>

namespace ava {

static float clamp01(float x) { return std::max(0.f, std::min(1.f, x)); }

void Detector::applyRangeMask(const cv::Mat& hsv, const ColorRangeHSV& r, cv::Mat& outMask) {
  int hMin = (r.hMin % 360 + 360) % 360;
  int hMax = (r.hMax % 360 + 360) % 360;
  int hMinCv = hMin / 2;
  int hMaxCv = hMax / 2;

  if (hMin <= hMax) {
    cv::inRange(hsv, cv::Scalar(hMinCv, r.sMin, r.vMin), cv::Scalar(hMaxCv, r.sMax, r.vMax), outMask);
  } else {
    cv::Mat m1, m2;
    cv::inRange(hsv, cv::Scalar(hMinCv, r.sMin, r.vMin), cv::Scalar(179, r.sMax, r.vMax), m1);
    cv::inRange(hsv, cv::Scalar(0, r.sMin, r.vMin), cv::Scalar(hMaxCv, r.sMax, r.vMax), m2);
    cv::bitwise_or(m1, m2, outMask);
  }
}

std::vector<Target> Detector::detect(const cv::Mat& bgr, const AppConfig& cfg, const QSize& roiSize) const {
  std::vector<Target> out;
  if (bgr.empty()) return out;

  cv::Mat hsv;
  cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);

  cv::Mat maskEnemy, maskFriend;
  applyRangeMask(hsv, cfg.enemyRange, maskEnemy);
  applyRangeMask(hsv, cfg.friendRange, maskFriend);

  int k = cfg.morphKernel;
  if (k < 1) k = 1;
  if (k % 2 == 0) k += 1;
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(k, k));

  cv::morphologyEx(maskEnemy, maskEnemy, cv::MORPH_OPEN, kernel);
  cv::morphologyEx(maskEnemy, maskEnemy, cv::MORPH_CLOSE, kernel);
  cv::morphologyEx(maskFriend, maskFriend, cv::MORPH_OPEN, kernel);
  cv::morphologyEx(maskFriend, maskFriend, cv::MORPH_CLOSE, kernel);

  auto extract = [&](const cv::Mat& mask, TargetKind kind) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (auto& c : contours) {
      double area = cv::contourArea(c);
      if (area < cfg.minContourArea) continue;
      cv::Rect r = cv::boundingRect(c);

      Target t;
      t.kind = kind;
      t.bbox = QRect(r.x, r.y, r.width, r.height);
      t.center = QPoint(r.x + r.width/2, r.y + r.height/2);

      float norm = float(area) / float(std::max(1, roiSize.width()*roiSize.height()));
      t.confidence = clamp01(norm * 12.f);
      out.push_back(t);
    }
  };

  extract(maskEnemy, TargetKind::Enemy);
  extract(maskFriend, TargetKind::Friend);

  std::sort(out.begin(), out.end(), [](const Target& a, const Target& b){
    return a.confidence > b.confidence;
  });

  if (out.size() > 6) out.resize(6);
  return out;
}

} // namespace ava
