#pragma once
#include <QWidget>
#include <QLabel>

namespace ava {

class StatsPage : public QWidget {
  Q_OBJECT
public:
  explicit StatsPage(QWidget* parent=nullptr);
  void setText(const QString& t);

private:
  QLabel* txt_ = nullptr;
};

} // namespace ava
