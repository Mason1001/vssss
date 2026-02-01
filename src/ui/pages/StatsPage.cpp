#include "ui/pages/StatsPage.h"
#include <QVBoxLayout>

namespace ava {

StatsPage::StatsPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(10);

  auto* t = new QLabel("الإحصائيات", this);
  t->setObjectName("PageTitle");

  txt_ = new QLabel("-", this);
  txt_->setWordWrap(true);

  root->addWidget(t);
  root->addWidget(txt_);
  root->addStretch(1);
}

void StatsPage::setText(const QString& t) { txt_->setText(t); }

} // namespace ava
