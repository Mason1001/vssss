#include "ui/widgets/HueSlider.h"
#include "vision/ColorMath.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <cmath>

namespace ava {

HueSlider::HueSlider(QWidget* parent) : QWidget(parent) {
  setMinimumHeight(22);
  setMouseTracking(true);
}

void HueSlider::setHue(float hueDeg) {
  hue_ = std::fmod(hueDeg, 360.f);
  if (hue_ < 0) hue_ += 360.f;
  update();
}

void HueSlider::paintEvent(QPaintEvent*) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);

  QLinearGradient g(0, 0, width(), 0);
  for (int i=0;i<=6;i++){
    float h = i * 60.f;
    g.setColorAt(i/6.0, HsvToRgb(HSVf{h, 1.f, 1.f}));
  }
  QRect r = rect().adjusted(0, 4, 0, -4);
  p.setPen(Qt::NoPen);
  p.setBrush(g);
  p.drawRoundedRect(r, 10, 10);

  int x = int((hue_ / 360.f) * (width()-1));
  QPoint c(x, rect().center().y());
  p.setBrush(Qt::white);
  p.setPen(QPen(QColor("#2D7DFF"), 2));
  p.drawEllipse(c, 8, 8);
}

void HueSlider::updateFromPos(const QPoint& pos) {
  float t = float(pos.x()) / float(std::max(1, width()-1));
  t = std::max(0.f, std::min(1.f, t));
  hue_ = t * 360.f;
  emit hueChanged(hue_);
  update();
}

void HueSlider::mousePressEvent(QMouseEvent* e) { if (e->button() == Qt::LeftButton) updateFromPos(e->pos()); }
void HueSlider::mouseMoveEvent(QMouseEvent* e) { if (e->buttons() & Qt::LeftButton) updateFromPos(e->pos()); }

} // namespace ava
