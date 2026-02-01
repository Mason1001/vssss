#include "ui/pages/ProfilesPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace ava {

ProfilesPage::ProfilesPage(ProfileManager* pm, QWidget* parent) : QWidget(parent), pm_(pm) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(12);

  auto* row = new QHBoxLayout();
  row->setSpacing(10);
  row->addWidget(new QLabel("اسم الملف:", this));
  name_ = new QLineEdit(this);
  name_->setPlaceholderText("مثال: مريض-001");
  row->addWidget(name_, 1);
  root->addLayout(row);

  list_ = new QListWidget(this);
  root->addWidget(list_, 1);

  auto* btns = new QHBoxLayout();
  btnSave_ = new QPushButton("حفظ/تحديث", this);
  btnLoad_ = new QPushButton("تحميل", this);
  btnDelete_ = new QPushButton("حذف", this);

  btns->addWidget(btnDelete_);
  btns->addWidget(btnLoad_);
  btns->addWidget(btnSave_);
  btns->addStretch(1);
  root->addLayout(btns);

  connect(btnSave_, &QPushButton::clicked, this, [this]{
    QString n = name_->text().trimmed();
    if (n.isEmpty() && list_->currentItem()) n = list_->currentItem()->text();
    if (n.isEmpty()) n = "افتراضي";
    emit requestSaveProfile(n);
  });

  connect(btnLoad_, &QPushButton::clicked, this, [this]{
    QString n = list_->currentItem() ? list_->currentItem()->text() : name_->text().trimmed();
    if (n.isEmpty()) return;
    emit requestLoadProfile(n);
  });

  connect(btnDelete_, &QPushButton::clicked, this, [this]{
    QString n = list_->currentItem() ? list_->currentItem()->text() : name_->text().trimmed();
    if (n.isEmpty()) return;
    emit requestDeleteProfile(n);
  });

  connect(list_, &QListWidget::currentTextChanged, this, [this](const QString& t){
    name_->setText(t);
  });

  refreshList();
}

void ProfilesPage::setConfig(const AppConfig& cfg) {
  cfg_ = cfg;
  name_->setText(cfg_.currentProfile);
}

void ProfilesPage::refreshList() {
  list_->clear();
  for (auto& p : pm_->listProfiles()) list_->addItem(p);
}

} // namespace ava
