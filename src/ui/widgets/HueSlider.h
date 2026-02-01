#pragma once
#include <QWidget>

namespace ava {

class HueSlider : public QWidget {
  Q_OBJECT
public:
  explicit HueSlider(QWidget* parent=nullptr);

  void setHue(float hueDeg);
  float hue() const { return hue_; }

signals:
  void hueChanged(float hueDeg);

protected:
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;

private:
  void updateFromPos(const QPoint& pos);
  float hue_ = 0.f;
};

} // namespace ava
