#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

#include "ui/widgets/SVPicker.h"
#include "ui/widgets/HueSlider.h"
#include "ui/widgets/ValueCard.h"

namespace ava {

class ColorPickerPanel : public QWidget {
  Q_OBJECT
public:
  explicit ColorPickerPanel(QWidget* parent=nullptr);

  QColor color() const { return color_; }
  void setColor(const QColor& c);

signals:
  void colorChanged(const QColor& c);

private slots:
  void onHue(float h);
  void onSV(float s, float v);
  void onHexEdited();

private:
  void refreshFields();

  QColor color_{255, 0, 0};
  float hue_ = 0.f;
  float s_ = 1.f;
  float v_ = 1.f;

  QFrame* preview_ = nullptr;
  SVPicker* sv_ = nullptr;
  HueSlider* hueSlider_ = nullptr;

  QLineEdit* hex_ = nullptr;
  QPushButton* copy_ = nullptr;

  ValueCard* rgbCard_ = nullptr;
  ValueCard* cmykCard_ = nullptr;
  ValueCard* hsvCard_ = nullptr;
  ValueCard* hslCard_ = nullptr;

  bool guard_ = false;
};

} // namespace ava
