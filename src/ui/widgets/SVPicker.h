#pragma once
#include <QWidget>

namespace ava {

// لوحة Saturation/Value
class SVPicker : public QWidget {
  Q_OBJECT
public:
  explicit SVPicker(QWidget* parent=nullptr);

  void setHue(float hueDeg);
  void setSV(float s, float v);
  float s() const { return s_; }
  float v() const { return v_; }

signals:
  void svChanged(float s, float v);

protected:
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;
  void resizeEvent(QResizeEvent*) override;

private:
  void updateFromPos(const QPoint& pos);

  float hue_ = 0.f;
  float s_ = 1.f;
  float v_ = 1.f;
  QImage cache_;
  bool cacheDirty_ = true;
};

} // namespace ava
