#include "ui/pages/DetectionPage.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>

namespace ava {

DetectionPage::DetectionPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(12);

  cbEnable_ = new QCheckBox("تفعيل الرصد", this);
  cbHighlight_ = new QCheckBox("تفعيل التمييز (Highlight)", this);
  cbAlert_ = new QCheckBox("تفعيل التنبيه الصوتي", this);

  root->addWidget(cbEnable_);
  root->addWidget(cbHighlight_);
  root->addWidget(cbAlert_);

  auto* gb = new QGroupBox("الفلاتر", this);
  auto* hl = new QHBoxLayout(gb);

  spMinArea_ = new QSpinBox(gb); spMinArea_->setRange(1, 50000); spMinArea_->setValue(60);
  spMorph_ = new QSpinBox(gb); spMorph_->setRange(1, 21); spMorph_->setSingleStep(2); spMorph_->setValue(3);

  hl->addWidget(new QLabel("أقل مساحة", gb));
  hl->addWidget(spMinArea_);
  hl->addSpacing(16);
  hl->addWidget(new QLabel("قوة الفلترة", gb));
  hl->addWidget(spMorph_);
  hl->addStretch(1);

  root->addWidget(gb);
  root->addStretch(1);

  auto apply = [this]{
    cfg_.detectionEnabled = cbEnable_->isChecked();
    cfg_.highlightEnabled = cbHighlight_->isChecked();
    cfg_.alertEnabled = cbAlert_->isChecked();
    cfg_.minContourArea = spMinArea_->value();
    cfg_.morphKernel = spMorph_->value();
    emit configChanged(cfg_);
  };

  connect(cbEnable_, &QCheckBox::toggled, this, [apply](bool){apply();});
  connect(cbHighlight_, &QCheckBox::toggled, this, [apply](bool){apply();});
  connect(cbAlert_, &QCheckBox::toggled, this, [apply](bool){apply();});
  connect(spMinArea_, qOverload<int>(&QSpinBox::valueChanged), this, [apply](int){apply();});
  connect(spMorph_, qOverload<int>(&QSpinBox::valueChanged), this, [apply](int){apply();});
}

void DetectionPage::setConfig(const AppConfig& cfg) {
  cfg_ = cfg;
  cbEnable_->setChecked(cfg_.detectionEnabled);
  cbHighlight_->setChecked(cfg_.highlightEnabled);
  cbAlert_->setChecked(cfg_.alertEnabled);
  spMinArea_->setValue(cfg_.minContourArea);
  spMorph_->setValue(cfg_.morphKernel);
}

} // namespace ava
