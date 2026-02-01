#pragma once
#include <QMainWindow>
#include <QTabBar>
#include <QStackedWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QTimer>

#include "app/AppConfig.h"
#include "app/ProfileManager.h"
#include "capture/ScreenCapture.h"
#include "vision/Detector.h"
#include "extensions/DefaultAssistExtension.h"
#include "overlay/OverlayWindow.h"

#include "ui/pages/DashboardPage.h"
#include "ui/pages/OverlayPage.h"
#include "ui/pages/ColorsPage.h"
#include "ui/pages/DetectionPage.h"
#include "ui/pages/ProfilesPage.h"
#include "ui/pages/HotkeysPage.h"
#include "ui/pages/StatsPage.h"
#include "ui/pages/SystemPage.h"

#ifdef _WIN32
#include "app/HotkeyManager_win.h"
#endif

namespace ava {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent=nullptr);
  ~MainWindow() override;

private:
  void buildUi();
  void applyTheme();
  void refreshProfileCombo();
  void applyConfigToAll();

  void startEngine();
  void stopEngine();
  void tick();

  QRect computeRoiGlobal() const;
  void pickColorFromScreenCenter();

  AppConfig cfg_;
  ProfileManager profiles_;

  QWidget* central_ = nullptr;
  QWidget* header_ = nullptr;
  QLabel* appTitle_ = nullptr;
  QLabel* status_ = nullptr;

  QTabBar* tabs_ = nullptr;
  QStackedWidget* stack_ = nullptr;

  QPushButton* btnRun_ = nullptr;
  QPushButton* btnOverlay_ = nullptr;
  QComboBox* cbProfiles_ = nullptr;
  QPushButton* btnQuickCalib_ = nullptr;

  DashboardPage* pageDash_ = nullptr;
  OverlayPage* pageOverlay_ = nullptr;
  ColorsPage* pageColors_ = nullptr;
  DetectionPage* pageDetect_ = nullptr;
  ProfilesPage* pageProfiles_ = nullptr;
  HotkeysPage* pageHotkeys_ = nullptr;
  StatsPage* pageStats_ = nullptr;
  SystemPage* pageSystem_ = nullptr;

  // Engine
  ScreenCapture capt_;
  Detector detector_;
  DefaultAssistExtension assist_;
  OverlayWindow* overlay_ = nullptr;
  QTimer engineTimer_;
  bool running_ = false;

#ifdef _WIN32
  HotkeyManagerWin hotkeys_;
#endif

  // Stats
  quint64 frames_ = 0;
  quint64 detections_ = 0;
  double avgMs_ = 0.0;

  // Tray
  QSystemTrayIcon tray_;
};

} // namespace ava
