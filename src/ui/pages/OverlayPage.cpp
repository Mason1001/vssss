#include "ui/pages/OverlayPage.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

namespace ava {

static QWidget* sliderRow(const QString& label, QSlider** out, int min, int max, QWidget* parent) {
  auto* w = new QWidget(parent);
  auto* h = new QHBoxLayout(w);
  h->setContentsMargins(0,0,0,0);
  h->setSpacing(10);

  auto* l = new QLabel(label, w);
  l->setMinimumWidth(120);

  auto* s = new QSlider(Qt::Horizontal, w);
  s->setRange(min, max);

  auto* v = new QLabel("-", w);
  v->setMinimumWidth(48);

  QObject::connect(s, &QSlider::valueChanged, w, [v](int x){ v->setText(QString::number(x)); });

  h->addWidget(l);
  h->addWidget(s, 1);
  h->addWidget(v);

  *out = s;
  return w;
}

OverlayPage::OverlayPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(12);

  cbOverlayEnable_ = new QCheckBox("تفعيل الـOverlay", this);
  cbOverlayVisible_ = new QCheckBox("إظهار الـOverlay", this);
  cbClickThrough_ = new QCheckBox("نقر-عبر (لا يلتقط الماوس)", this);

  root->addWidget(cbOverlayEnable_);
  root->addWidget(cbOverlayVisible_);
  root->addWidget(cbClickThrough_);

  root->addWidget(sliderRow("الشفافية", &slOpacity_, 40, 255, this));
  root->addWidget(sliderRow("حجم FOV", &slFov_, 60, 600, this));
  root->addWidget(sliderRow("المنطقة الداخلية", &slInner_, 20, 400, this));
  root->addWidget(sliderRow("سُمك الدائرة", &slThickness_, 1, 10, this));

  root->addStretch(1);

  auto apply = [this]{
    cfg_.overlayEnabled = cbOverlayEnable_->isChecked();
    cfg_.overlayVisible = cbOverlayVisible_->isChecked();
    cfg_.overlayClickThrough = cbClickThrough_->isChecked();
    cfg_.overlayOpacity = slOpacity_->value();
    cfg_.fovRadius = slFov_->value();
    cfg_.fovInnerRadius = slInner_->value();
    cfg_.fovThickness = slThickness_->value();
    emit configChanged(cfg_);
  };

  connect(cbOverlayEnable_, &QCheckBox::toggled, this, [apply](bool){apply();});
  connect(cbOverlayVisible_, &QCheckBox::toggled, this, [apply](bool){apply();});
  connect(cbClickThrough_, &QCheckBox::toggled, this, [apply](bool){apply();});

  connect(slOpacity_, &QSlider::valueChanged, this, [apply](int){apply();});
  connect(slFov_, &QSlider::valueChanged, this, [apply](int){apply();});
  connect(slInner_, &QSlider::valueChanged, this, [apply](int){apply();});
  connect(slThickness_, &QSlider::valueChanged, this, [apply](int){apply();});
}

void OverlayPage::setConfig(const AppConfig& cfg) {
  cfg_ = cfg;
  cbOverlayEnable_->setChecked(cfg_.overlayEnabled);
  cbOverlayVisible_->setChecked(cfg_.overlayVisible);
  cbClickThrough_->setChecked(cfg_.overlayClickThrough);
  slOpacity_->setValue(cfg_.overlayOpacity);
  slFov_->setValue(cfg_.fovRadius);
  slInner_->setValue(cfg_.fovInnerRadius);
  slThickness_->setValue(cfg_.fovThickness);
}

} // namespace ava
