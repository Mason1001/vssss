#include "ui/widgets/ValueCard.h"
#include <QVBoxLayout>

namespace ava {

ValueCard::ValueCard(const QString& title, QWidget* parent) : QFrame(parent) {
  setObjectName("ValueCard");
  setFrameShape(QFrame::NoFrame);

  auto* lay = new QVBoxLayout(this);
  lay->setContentsMargins(12, 10, 12, 10);
  lay->setSpacing(6);

  title_ = new QLabel(title, this);
  title_->setObjectName("ValueCardTitle");
  value_ = new QLabel("-", this);
  value_->setObjectName("ValueCardValue");

  lay->addWidget(title_);
  lay->addWidget(value_);
}

void ValueCard::setValue(const QString& v) { value_->setText(v); }

} // namespace ava
