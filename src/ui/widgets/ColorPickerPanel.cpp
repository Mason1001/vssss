#include "ui/widgets/ColorPickerPanel.h"
#include "vision/ColorMath.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QClipboard>
#include <QApplication>
#include <QRegularExpression>
#include <cmath>

namespace ava {

static QString fmtRGB(const QColor& c) {
  return QString("%1, %2, %3").arg(c.red()).arg(c.green()).arg(c.blue());
}
static QString fmtHSV(const HSVf& hsv) {
  return QString("%1°, %2%, %3%")
      .arg(int(std::round(hsv.h)))
      .arg(int(std::round(hsv.s*100.f)))
      .arg(int(std::round(hsv.v*100.f)));
}
static QString fmtHSL(const HSLf& hsl) {
  return QString("%1°, %2%, %3%")
      .arg(int(std::round(hsl.h)))
      .arg(int(std::round(hsl.s*100.f)))
      .arg(int(std::round(hsl.l*100.f)));
}
static QString fmtCMYK(const CMYKf& c) {
  return QString("%1%, %2%, %3%, %4%")
      .arg(int(std::round(c.c*100.f)))
      .arg(int(std::round(c.m*100.f)))
      .arg(int(std::round(c.y*100.f)))
      .arg(int(std::round(c.k*100.f)));
}

ColorPickerPanel::ColorPickerPanel(QWidget* parent) : QWidget(parent) {
  setObjectName("ColorPickerPanel");
  setLayoutDirection(Qt::RightToLeft);

  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(14, 14, 14, 14);
  root->setSpacing(12);

  auto* top = new QHBoxLayout();
  top->setSpacing(12);

  preview_ = new QFrame(this);
  preview_->setFixedWidth(120);
  preview_->setObjectName("ColorPreview");

  sv_ = new SVPicker(this);
  hueSlider_ = new HueSlider(this);

  auto* right = new QVBoxLayout();
  right->setSpacing(10);
  right->addWidget(sv_, 1);
  right->addWidget(hueSlider_);

  top->addLayout(right, 1);
  top->addWidget(preview_);

  root->addLayout(top);

  auto* hexRow = new QHBoxLayout();
  hexRow->setSpacing(10);

  auto* hexLabel = new QLabel("HEX", this);
  hexLabel->setObjectName("HexLabel");

  hex_ = new QLineEdit(this);
  hex_->setObjectName("HexInput");
  hex_->setPlaceholderText("#RRGGBB");
  hex_->setMaxLength(7);

  copy_ = new QPushButton("نسخ", this);
  copy_->setObjectName("CopyButton");

  hexRow->addWidget(copy_);
  hexRow->addWidget(hex_, 1);
  hexRow->addWidget(hexLabel);

  root->addLayout(hexRow);

  auto* grid = new QGridLayout();
  grid->setHorizontalSpacing(10);
  grid->setVerticalSpacing(10);

  rgbCard_ = new ValueCard("RGB", this);
  cmykCard_ = new ValueCard("CMYK", this);
  hsvCard_ = new ValueCard("HSV", this);
  hslCard_ = new ValueCard("HSL", this);

  grid->addWidget(rgbCard_, 0, 0);
  grid->addWidget(cmykCard_, 0, 1);
  grid->addWidget(hsvCard_, 1, 0);
  grid->addWidget(hslCard_, 1, 1);

  root->addLayout(grid);

  connect(hueSlider_, &HueSlider::hueChanged, this, &ColorPickerPanel::onHue);
  connect(sv_, &SVPicker::svChanged, this, &ColorPickerPanel::onSV);
  connect(hex_, &QLineEdit::editingFinished, this, &ColorPickerPanel::onHexEdited);

  connect(copy_, &QPushButton::clicked, this, [this]{
    QApplication::clipboard()->setText(hex_->text());
  });

  setColor(color_);
}

void ColorPickerPanel::setColor(const QColor& c) {
  guard_ = true;
  color_ = c;
  auto hsv = RgbToHsv(color_);
  auto hsl = RgbToHsl(color_);
  hue_ = hsv.h; s_ = hsv.s; v_ = hsv.v;
  hueSlider_->setHue(hue_);
  sv_->setHue(hue_);
  sv_->setSV(s_, v_);
  guard_ = false;
  Q_UNUSED(hsl);
  refreshFields();
}

void ColorPickerPanel::onHue(float h) {
  if (guard_) return;
  hue_ = h;
  sv_->setHue(hue_);
  color_ = HsvToRgb(HSVf{hue_, s_, v_});
  refreshFields();
  emit colorChanged(color_);
}

void ColorPickerPanel::onSV(float s, float v) {
  if (guard_) return;
  s_ = s; v_ = v;
  color_ = HsvToRgb(HSVf{hue_, s_, v_});
  refreshFields();
  emit colorChanged(color_);
}

void ColorPickerPanel::onHexEdited() {
  if (guard_) return;
  const QString t = hex_->text().trimmed();
  QRegularExpression re("^#?[0-9A-Fa-f]{6}$");
  if (!re.match(t).hasMatch()) { refreshFields(); return; }
  QString x = t;
  if (!x.startsWith("#")) x.prepend("#");
  QColor c(x);
  if (!c.isValid()) { refreshFields(); return; }
  setColor(c);
  emit colorChanged(color_);
}

void ColorPickerPanel::refreshFields() {
  preview_->setStyleSheet(QString("background:%1; border-radius:14px; border:1px solid #23262D;").arg(ToHex(color_)));

  guard_ = true;
  hex_->setText(ToHex(color_));
  guard_ = false;

  auto hsv = RgbToHsv(color_);
  auto hsl = RgbToHsl(color_);
  auto cmyk = RgbToCmyk(color_);

  rgbCard_->setValue(fmtRGB(color_));
  cmykCard_->setValue(fmtCMYK(cmyk));
  hsvCard_->setValue(fmtHSV(hsv));
  hslCard_->setValue(fmtHSL(hsl));
}

} // namespace ava
