#pragma once
#include <QWidget>
#include "app/AppConfig.h"

class QComboBox;
class QSlider;

namespace ava {

class SystemPage : public QWidget {
  Q_OBJECT
public:
  explicit SystemPage(QWidget* parent=nullptr);
  void setConfig(const AppConfig& cfg);

signals:
  void configChanged(const AppConfig& cfg);

private:
  AppConfig cfg_;
  QComboBox* screens_ = nullptr;
  QSlider* fps_ = nullptr;
};

} // namespace ava
