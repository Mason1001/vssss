#pragma once
#include <QWidget>
#include "app/AppConfig.h"

class QSlider;
class QCheckBox;

namespace ava {

class OverlayPage : public QWidget {
  Q_OBJECT
public:
  explicit OverlayPage(QWidget* parent=nullptr);
  void setConfig(const AppConfig& cfg);

signals:
  void configChanged(const AppConfig& cfg);

private:
  AppConfig cfg_;

  QCheckBox* cbOverlayEnable_ = nullptr;
  QCheckBox* cbOverlayVisible_ = nullptr;
  QCheckBox* cbClickThrough_ = nullptr;

  QSlider* slOpacity_ = nullptr;
  QSlider* slFov_ = nullptr;
  QSlider* slInner_ = nullptr;
  QSlider* slThickness_ = nullptr;
};

} // namespace ava
