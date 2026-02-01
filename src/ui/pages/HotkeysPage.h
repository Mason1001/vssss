#pragma once
#include <QWidget>
#include "app/AppConfig.h"

class QLabel;

namespace ava {

class HotkeysPage : public QWidget {
  Q_OBJECT
public:
  explicit HotkeysPage(QWidget* parent=nullptr);
  void setConfig(const AppConfig& cfg);

private:
  QLabel* info_ = nullptr;
};

} // namespace ava
