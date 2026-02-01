#pragma once
#include <QWidget>
#include "app/AppConfig.h"
#include "ui/widgets/ColorPickerPanel.h"

class QRadioButton;
class QSpinBox;
class QPushButton;

namespace ava {

class ColorsPage : public QWidget {
  Q_OBJECT
public:
  explicit ColorsPage(QWidget* parent=nullptr);
  void setConfig(const AppConfig& cfg);

signals:
  void configChanged(const AppConfig& cfg);
  void requestPickFromScreen();

private:
  AppConfig cfg_;
  ColorPickerPanel* picker_ = nullptr;
  QRadioButton* rbEnemy_ = nullptr;
  QRadioButton* rbFriend_ = nullptr;
  QSpinBox* spHueTol_ = nullptr;
  QSpinBox* spSatTol_ = nullptr;
  QSpinBox* spValTol_ = nullptr;
  QPushButton* btnApplyRange_ = nullptr;
  QPushButton* btnPickScreen_ = nullptr;
  void applyRangeFromPicker();
};

} // namespace ava
