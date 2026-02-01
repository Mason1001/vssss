#pragma once
#include "app/AppConfig.h"
#include <QRect>
#include <QPoint>
#include <QSize>
#include <vector>

#include <opencv2/core.hpp>

namespace ava {

enum class TargetKind { Enemy, Friend };

struct Target {
  TargetKind kind = TargetKind::Enemy;
  QRect bbox;
  QPoint center;
  float confidence = 0.f;
};

class Detector {
public:
  std::vector<Target> detect(const cv::Mat& bgr, const AppConfig& cfg, const QSize& roiSize) const;

private:
  static void applyRangeMask(const cv::Mat& hsv, const ColorRangeHSV& r, cv::Mat& outMask);
};

} // namespace ava
