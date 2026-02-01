#pragma once
#include <QWidget>
#include "app/AppConfig.h"
#include "vision/Detector.h"

namespace ava {

class OverlayWindow : public QWidget {
  Q_OBJECT
public:
  explicit OverlayWindow(QWidget* parent = nullptr);

  void applyConfig(const AppConfig& cfg);
  void setTargets(const std::vector<Target>& targets, const QRect& roiGlobalRect);
  void setRunning(bool running);

protected:
  void paintEvent(QPaintEvent* e) override;
  void showEvent(QShowEvent* e) override;

private:
  void applyClickThroughWin(bool enable);

  AppConfig cfg_;
  bool running_ = false;
  std::vector<Target> targets_;
  QRect roiGlobal_;
};

} // namespace ava
