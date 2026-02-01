#include "ui/widgets/SVPicker.h"
#include "vision/ColorMath.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

namespace ava {

SVPicker::SVPicker(QWidget* parent) : QWidget(parent) {
  setMinimumSize(240, 240);
  setMouseTracking(true);
}

void SVPicker::setHue(float hueDeg) { hue_ = hueDeg; cacheDirty_ = true; update(); }

void SVPicker::setSV(float s, float v) {
  s_ = std::max(0.f, std::min(1.f, s));
  v_ = std::max(0.f, std::min(1.f, v));
  update();
}

void SVPicker::resizeEvent(QResizeEvent*) { cacheDirty_ = true; }

void SVPicker::paintEvent(QPaintEvent*) {
  if (cacheDirty_ || cache_.size() != size()) {
    cache_ = QImage(size(), QImage::Format_RGBA8888);
    for (int y=0; y<cache_.height(); ++y) {
      float v = 1.f - (float(y) / float(std::max(1, cache_.height()-1)));
      for (int x=0; x<cache_.width(); ++x) {
        float s = float(x) / float(std::max(1, cache_.width()-1));
        cache_.setPixelColor(x, y, HsvToRgb(HSVf{hue_, s, v}));
      }
    }
    cacheDirty_ = false;
  }

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.drawImage(rect(), cache_);

  int x = int(s_ * (width()-1));
  int y = int((1.f - v_) * (height()-1));

  QPen pen(Qt::white); pen.setWidth(2);
  p.setPen(pen);
  p.setBrush(Qt::NoBrush);
  p.drawEllipse(QPoint(x,y), 7, 7);

  QPen pen2(QColor("#2D7DFF")); pen2.setWidth(2);
  p.setPen(pen2);
  p.drawEllipse(QPoint(x,y), 9, 9);
}

void SVPicker::updateFromPos(const QPoint& pos) {
  float s = float(pos.x()) / float(std::max(1, width()-1));
  float v = 1.f - (float(pos.y()) / float(std::max(1, height()-1)));
  s = std::max(0.f, std::min(1.f, s));
  v = std::max(0.f, std::min(1.f, v));
  s_ = s; v_ = v;
  emit svChanged(s_, v_);
  update();
}

void SVPicker::mousePressEvent(QMouseEvent* e) { if (e->button() == Qt::LeftButton) updateFromPos(e->pos()); }
void SVPicker::mouseMoveEvent(QMouseEvent* e) { if (e->buttons() & Qt::LeftButton) updateFromPos(e->pos()); }

} // namespace ava
