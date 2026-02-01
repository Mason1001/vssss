#include "ui/pages/ColorsPage.h"
#include "vision/ColorRange.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

namespace ava {

ColorsPage::ColorsPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(12);

  picker_ = new ColorPickerPanel(this);
  root->addWidget(picker_);

  auto* gb = new QGroupBox("تطبيق النطاق على", this);
  auto* gbl = new QHBoxLayout(gb);
  rbEnemy_ = new QRadioButton("عدو", gb);
  rbFriend_ = new QRadioButton("صديق", gb);
  rbEnemy_->setChecked(true);
  gbl->addWidget(rbEnemy_);
  gbl->addWidget(rbFriend_);
  gbl->addStretch(1);
  root->addWidget(gb);

  auto* tol = new QGroupBox("سماح النطاق (Tolerance)", this);
  auto* tl = new QHBoxLayout(tol);

  spHueTol_ = new QSpinBox(tol); spHueTol_->setRange(1, 60); spHueTol_->setValue(12);
  spSatTol_ = new QSpinBox(tol); spSatTol_->setRange(0, 255); spSatTol_->setValue(60);
  spValTol_ = new QSpinBox(tol); spValTol_->setRange(0, 255); spValTol_->setValue(60);

  tl->addWidget(new QLabel("Hue°", tol));
  tl->addWidget(spHueTol_);
  tl->addSpacing(12);
  tl->addWidget(new QLabel("S", tol));
  tl->addWidget(spSatTol_);
  tl->addSpacing(12);
  tl->addWidget(new QLabel("V", tol));
  tl->addWidget(spValTol_);
  tl->addStretch(1);

  root->addWidget(tol);

  auto* actions = new QHBoxLayout();
  btnApplyRange_ = new QPushButton("تطبيق النطاق", this);
  btnPickScreen_ = new QPushButton("التقاط من الشاشة", this);
  actions->addWidget(btnPickScreen_);
  actions->addWidget(btnApplyRange_);
  actions->addStretch(1);
  root->addLayout(actions);

  root->addStretch(1);

  connect(btnApplyRange_, &QPushButton::clicked, this, &ColorsPage::applyRangeFromPicker);
  connect(btnPickScreen_, &QPushButton::clicked, this, &ColorsPage::requestPickFromScreen);
}

void ColorsPage::setConfig(const AppConfig& cfg) { cfg_ = cfg; }

void ColorsPage::applyRangeFromPicker() {
  const QColor c = picker_->color();
  auto range = MakeRangeFromColor(c, spHueTol_->value(), spSatTol_->value(), spValTol_->value());
  if (rbEnemy_->isChecked()) cfg_.enemyRange = range.r;
  else cfg_.friendRange = range.r;
  emit configChanged(cfg_);
}

} // namespace ava
