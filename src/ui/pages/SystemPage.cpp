#include "ui/pages/SystemPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QGuiApplication>
#include <QScreen>

namespace ava {

SystemPage::SystemPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(12);

  auto* row1 = new QHBoxLayout();
  row1->addWidget(new QLabel("الشاشة:", this));
  screens_ = new QComboBox(this);
  row1->addWidget(screens_, 1);
  root->addLayout(row1);

  auto* row2 = new QHBoxLayout();
  row2->addWidget(new QLabel("سرعة الالتقاط (FPS):", this));
  fps_ = new QSlider(Qt::Horizontal, this);
  fps_->setRange(5, 60);
  row2->addWidget(fps_, 1);
  auto* val = new QLabel("-", this);
  val->setMinimumWidth(40);
  row2->addWidget(val);
  root->addLayout(row2);

  connect(fps_, &QSlider::valueChanged, this, [val](int x){ val->setText(QString::number(x)); });

  // screens
  screens_->clear();
  auto screens = QGuiApplication::screens();
  for (int i=0; i<screens.size(); ++i) {
    auto g = screens[i]->geometry();
    screens_->addItem(QString("شاشة %1 (%2x%3)").arg(i+1).arg(g.width()).arg(g.height()), i);
  }

  connect(screens_, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int idx){
    cfg_.selectedScreenIndex = idx;
    emit configChanged(cfg_);
  });
  connect(fps_, &QSlider::valueChanged, this, [this](int v){
    cfg_.captureFps = v;
    emit configChanged(cfg_);
  });

  root->addStretch(1);
}

void SystemPage::setConfig(const AppConfig& cfg) {
  cfg_ = cfg;
  screens_->setCurrentIndex(cfg_.selectedScreenIndex);
  fps_->setValue(cfg_.captureFps);
}

} // namespace ava
