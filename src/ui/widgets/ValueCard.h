#pragma once
#include <QFrame>
#include <QLabel>

namespace ava {

class ValueCard : public QFrame {
  Q_OBJECT
public:
  explicit ValueCard(const QString& title, QWidget* parent=nullptr);
  void setValue(const QString& v);

private:
  QLabel* title_ = nullptr;
  QLabel* value_ = nullptr;
};

} // namespace ava
