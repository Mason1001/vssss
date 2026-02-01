#pragma once
#include <QWidget>
#include "app/AppConfig.h"

class QCheckBox;
class QSpinBox;

namespace ava {

class DetectionPage : public QWidget {
  Q_OBJECT
public:
  explicit DetectionPage(QWidget* parent=nullptr);
  void setConfig(const AppConfig& cfg);

signals:
  void configChanged(const AppConfig& cfg);

private:
  AppConfig cfg_;
  QCheckBox* cbEnable_ = nullptr;
  QCheckBox* cbHighlight_ = nullptr;
  QCheckBox* cbAlert_ = nullptr;
  QSpinBox* spMinArea_ = nullptr;
  QSpinBox* spMorph_ = nullptr;
};

} // namespace ava
