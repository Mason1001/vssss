#pragma once
#include <QWidget>
#include <QLabel>

namespace ava {

class DashboardPage : public QWidget {
  Q_OBJECT
public:
  explicit DashboardPage(QWidget* parent=nullptr);
  void setStatusText(const QString& s);
  void setPerfText(const QString& s);

private:
  QLabel* status_ = nullptr;
  QLabel* perf_ = nullptr;
};

} // namespace ava
