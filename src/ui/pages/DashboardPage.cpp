#include "ui/pages/DashboardPage.h"
#include <QVBoxLayout>
#include <QFrame>

namespace ava {

static QFrame* card(const QString& title, QLabel** outValue, QWidget* parent) {
  auto* f = new QFrame(parent);
  f->setObjectName("Card");
  auto* l = new QVBoxLayout(f);
  l->setContentsMargins(14, 12, 14, 12);
  l->setSpacing(8);
  auto* t = new QLabel(title, f);
  t->setObjectName("CardTitle");
  auto* v = new QLabel("-", f);
  v->setObjectName("CardValue");
  l->addWidget(t);
  l->addWidget(v);
  *outValue = v;
  return f;
}

DashboardPage::DashboardPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16, 16, 16, 16);
  root->setSpacing(12);

  QLabel* s=nullptr; QLabel* p=nullptr;
  auto* c1 = card("الحالة", &s, this);
  auto* c2 = card("الأداء", &p, this);

  status_ = s;
  perf_ = p;

  root->addWidget(c1);
  root->addWidget(c2);
  root->addStretch(1);
}

void DashboardPage::setStatusText(const QString& s) { status_->setText(s); }
void DashboardPage::setPerfText(const QString& s) { perf_->setText(s); }

} // namespace ava
