#include "ui/MainWindow.h"
#include "app/Logger.h"
#include "app/AppConfig.h"
#include "vision/ColorRange.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include <QElapsedTimer>
#include <QGuiApplication>
#include <QScreen>

#include <opencv2/imgproc.hpp>

namespace ava {

static QIcon iconFromRes(const QString& path) { return QIcon(path); }

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent), cfg_(AppConfigIO::Load()), profiles_() {

  setWindowTitle("مساعد الرؤية");
  setLayoutDirection(Qt::RightToLeft);
  resize(1180, 720);

  buildUi();
  applyTheme();
  refreshProfileCombo();

  overlay_ = new OverlayWindow();
  overlay_->applyConfig(cfg_);
  overlay_->show();

  connect(&engineTimer_, &QTimer::timeout, this, &MainWindow::tick);
  engineTimer_.setTimerType(Qt::PreciseTimer);

#ifdef _WIN32
  qApp->installNativeEventFilter(&hotkeys_);
  hotkeys_.registerHotkeys(cfg_.hkToggleRun, cfg_.hkToggleOverlay, cfg_.hkPickColor);
  connect(&hotkeys_, &HotkeyManagerWin::sigToggleRun, this, [this]{ running_ ? stopEngine() : startEngine(); });
  connect(&hotkeys_, &HotkeyManagerWin::sigToggleOverlay, this, [this]{
    cfg_.overlayVisible = !cfg_.overlayVisible;
    applyConfigToAll();
    AppConfigIO::Save(cfg_);
  });
  connect(&hotkeys_, &HotkeyManagerWin::sigPickColor, this, [this]{ pickColorFromScreenCenter(); });
#endif

  // Tray
  tray_.setIcon(QIcon(":/resources/icons/dashboard.svg"));
  tray_.setToolTip("مساعد الرؤية");
  auto* menu = new QMenu();
  auto* aShow = menu->addAction("إظهار");
  auto* aHide = menu->addAction("إخفاء");
  menu->addSeparator();
  auto* aStart = menu->addAction("بدء/إيقاف");
  menu->addSeparator();
  auto* aExit = menu->addAction("خروج");

  connect(aShow, &QAction::triggered, this, [this]{ showNormal(); raise(); activateWindow(); });
  connect(aHide, &QAction::triggered, this, [this]{ hide(); });
  connect(aStart, &QAction::triggered, this, [this]{ running_ ? stopEngine() : startEngine(); });
  connect(aExit, &QAction::triggered, this, [this]{ close(); });

  tray_.setContextMenu(menu);
  tray_.show();

  applyConfigToAll();
}

MainWindow::~MainWindow() {
  stopEngine();
  AppConfigIO::Save(cfg_);
  if (overlay_) overlay_->close();
#ifdef _WIN32
  hotkeys_.unregisterHotkeys();
  qApp->removeNativeEventFilter(&hotkeys_);
#endif
}

void MainWindow::buildUi() {
  central_ = new QWidget(this);
  setCentralWidget(central_);

  auto* root = new QVBoxLayout(central_);
  root->setContentsMargins(14, 14, 14, 14);
  root->setSpacing(12);

  // Header
  header_ = new QWidget(central_);
  header_->setObjectName("Header");
  auto* hl = new QHBoxLayout(header_);
  hl->setContentsMargins(14, 12, 14, 12);
  hl->setSpacing(12);

  appTitle_ = new QLabel("مساعد الرؤية", header_);
  appTitle_->setObjectName("AppTitle");

  status_ = new QLabel("متوقف", header_);
  status_->setObjectName("StatusBadge");

  tabs_ = new QTabBar(header_);
  tabs_->setObjectName("TopTabs");
  tabs_->setExpanding(false);
  tabs_->setDrawBase(false);
  tabs_->setUsesScrollButtons(true);
  tabs_->setElideMode(Qt::ElideNone);

  auto addTab = [&](const QString& text, const QString& iconRes){
    int idx = tabs_->addTab(iconFromRes(iconRes), text);
    tabs_->setTabToolTip(idx, text);
    return idx;
  };

  addTab("لوحة التحكم", ":/resources/icons/dashboard.svg");
  addTab("الواجهة وFOV", ":/resources/icons/overlay.svg");
  addTab("الألوان", ":/resources/icons/colors.svg");
  addTab("الرصد والفلاتر", ":/resources/icons/detect.svg");
  addTab("الملفات الشخصية", ":/resources/icons/profiles.svg");
  addTab("الأزرار", ":/resources/icons/hotkeys.svg");
  addTab("الإحصائيات", ":/resources/icons/stats.svg");
  addTab("النظام", ":/resources/icons/system.svg");

  btnRun_ = new QPushButton("بدء", header_);
  btnRun_->setObjectName("PrimaryButton");

  btnOverlay_ = new QPushButton("إظهار/إخفاء", header_);
  btnOverlay_->setObjectName("SecondaryButton");

  cbProfiles_ = new QComboBox(header_);
  cbProfiles_->setObjectName("ProfileCombo");

  btnQuickCalib_ = new QPushButton("التقاط لون (مركز)", header_);
  btnQuickCalib_->setObjectName("SecondaryButton");

  hl->addWidget(appTitle_);
  hl->addWidget(status_);
  hl->addWidget(tabs_, 1);
  hl->addWidget(btnRun_);
  hl->addWidget(btnOverlay_);
  hl->addWidget(cbProfiles_);
  hl->addWidget(btnQuickCalib_);
  root->addWidget(header_);

  stack_ = new QStackedWidget(central_);
  stack_->setObjectName("Stack");

  pageDash_ = new DashboardPage(stack_);
  pageOverlay_ = new OverlayPage(stack_);
  pageColors_ = new ColorsPage(stack_);
  pageDetect_ = new DetectionPage(stack_);
  pageProfiles_ = new ProfilesPage(&profiles_, stack_);
  pageHotkeys_ = new HotkeysPage(stack_);
  pageStats_ = new StatsPage(stack_);
  pageSystem_ = new SystemPage(stack_);

  stack_->addWidget(pageDash_);
  stack_->addWidget(pageOverlay_);
  stack_->addWidget(pageColors_);
  stack_->addWidget(pageDetect_);
  stack_->addWidget(pageProfiles_);
  stack_->addWidget(pageHotkeys_);
  stack_->addWidget(pageStats_);
  stack_->addWidget(pageSystem_);

  root->addWidget(stack_, 1);

  connect(tabs_, &QTabBar::currentChanged, stack_, &QStackedWidget::setCurrentIndex);
  tabs_->setCurrentIndex(0);

  connect(btnRun_, &QPushButton::clicked, this, [this]{ running_ ? stopEngine() : startEngine(); });
  connect(btnOverlay_, &QPushButton::clicked, this, [this]{
    cfg_.overlayVisible = !cfg_.overlayVisible;
    applyConfigToAll();
    AppConfigIO::Save(cfg_);
  });
  connect(btnQuickCalib_, &QPushButton::clicked, this, [this]{ pickColorFromScreenCenter(); });

  connect(cbProfiles_, &QComboBox::currentTextChanged, this, [this](const QString& name){
    if (name.isEmpty()) return;
    AppConfig loaded;
    if (profiles_.loadProfile(name, loaded)) {
      cfg_ = loaded;
      applyConfigToAll();
      AppConfigIO::Save(cfg_);
    }
  });

  connect(pageOverlay_, &OverlayPage::configChanged, this, [this](const AppConfig& c){
    cfg_ = c; applyConfigToAll(); AppConfigIO::Save(cfg_);
  });
  connect(pageDetect_, &DetectionPage::configChanged, this, [this](const AppConfig& c){
    cfg_ = c; applyConfigToAll(); AppConfigIO::Save(cfg_);
  });
  connect(pageSystem_, &SystemPage::configChanged, this, [this](const AppConfig& c){
    cfg_ = c; applyConfigToAll(); AppConfigIO::Save(cfg_);
  });
  connect(pageColors_, &ColorsPage::configChanged, this, [this](const AppConfig& c){
    cfg_ = c; applyConfigToAll(); AppConfigIO::Save(cfg_);
  });
  connect(pageColors_, &ColorsPage::requestPickFromScreen, this, [this]{ pickColorFromScreenCenter(); });

  connect(pageProfiles_, &ProfilesPage::requestSaveProfile, this, [this](const QString& name){
    profiles_.saveProfile(name, cfg_);
    cfg_.currentProfile = name;
    refreshProfileCombo();
    pageProfiles_->refreshList();
    AppConfigIO::Save(cfg_);
    applyConfigToAll();
  });
  connect(pageProfiles_, &ProfilesPage::requestLoadProfile, this, [this](const QString& name){
    AppConfig loaded;
    if (profiles_.loadProfile(name, loaded)) {
      cfg_ = loaded;
      applyConfigToAll();
      AppConfigIO::Save(cfg_);
    }
  });
  connect(pageProfiles_, &ProfilesPage::requestDeleteProfile, this, [this](const QString& name){
    profiles_.deleteProfile(name);
    refreshProfileCombo();
    pageProfiles_->refreshList();
  });
}

void MainWindow::applyTheme() {
  QString qss = R"(
    QWidget { font-family: "Segoe UI"; font-size: 13px; color: #EAEAEA; }
    QMainWindow { background: #0E0F12; }
    #Header { background: #13151A; border: 1px solid #23262D; border-radius: 16px; }
    #AppTitle { font-size: 18px; font-weight: 700; }
    #StatusBadge { padding: 6px 10px; border-radius: 10px; background: #1E222B; border: 1px solid #2A2F3A; color: #C8D1E6; }
    #TopTabs::tab { background: transparent; padding: 8px 12px; margin: 0 4px; border-radius: 12px; }
    #TopTabs::tab:selected { background: rgba(45,125,255,0.18); border: 1px solid rgba(45,125,255,0.35); }
    #TopTabs::tab:hover { background: rgba(255,255,255,0.06); }
    #PrimaryButton { background: #2D7DFF; border: 1px solid #2D7DFF; color: white; padding: 8px 14px; border-radius: 12px; font-weight: 600; }
    #PrimaryButton:hover { background: #3A88FF; }
    #SecondaryButton { background: #1A1D24; border: 1px solid #2A2F3A; padding: 8px 14px; border-radius: 12px; }
    #SecondaryButton:hover { background: #202431; }
    #ProfileCombo { background: #141720; border: 1px solid #2A2F3A; padding: 6px 10px; border-radius: 12px; min-width: 170px; }
    #Stack { background: #0E0F12; }
    QGroupBox { border: 1px solid #23262D; border-radius: 16px; margin-top: 10px; padding: 12px; background: #11131A; }
    QGroupBox::title { subcontrol-origin: margin; right: 14px; padding: 0 6px; color: #C8D1E6; }
    QCheckBox { padding: 6px; }
    QSlider::groove:horizontal { height: 8px; background: #1A1D24; border: 1px solid #2A2F3A; border-radius: 8px; }
    QSlider::handle:horizontal { width: 18px; margin: -6px 0; border-radius: 9px; background: #2D7DFF; border: 2px solid #EAEAEA; }
    #ColorPickerPanel { background: #11131A; border: 1px solid #23262D; border-radius: 18px; }
    #HexInput { background: #141720; border: 1px solid #2A2F3A; border-radius: 12px; padding: 8px 10px; }
    #CopyButton { background: #1A1D24; border: 1px solid #2A2F3A; border-radius: 12px; padding: 8px 14px; }
    #CopyButton:hover { background: #202431; }
    #HexLabel { color: #C8D1E6; font-weight: 700; }
    #ValueCard { background: #141720; border: 1px solid #2A2F3A; border-radius: 16px; }
    #ValueCardTitle { color: #C8D1E6; font-weight: 700; }
    #Card { background: #11131A; border: 1px solid #23262D; border-radius: 16px; }
    #CardTitle { color: #C8D1E6; font-weight: 700; }
    #PageTitle { font-size: 16px; font-weight: 700; }
  )";
  qApp->setStyleSheet(qss);
}

void MainWindow::refreshProfileCombo() {
  cbProfiles_->blockSignals(true);
  cbProfiles_->clear();
  auto list = profiles_.listProfiles();
  cbProfiles_->addItems(list);
  int idx = cbProfiles_->findText(cfg_.currentProfile);
  if (idx >= 0) cbProfiles_->setCurrentIndex(idx);
  cbProfiles_->blockSignals(false);
}

void MainWindow::applyConfigToAll() {
  cfg_.fovInnerRadius = std::min(cfg_.fovInnerRadius, cfg_.fovRadius - 10);
  cfg_.selectedScreenIndex = std::max(0, cfg_.selectedScreenIndex);

  overlay_->applyConfig(cfg_);
  overlay_->setRunning(running_);

  pageOverlay_->setConfig(cfg_);
  pageDetect_->setConfig(cfg_);
  pageSystem_->setConfig(cfg_);
  pageHotkeys_->setConfig(cfg_);
  pageProfiles_->setConfig(cfg_);

  pageDash_->setStatusText(QString("الملف: %1\nOverlay: %2\nالرصد: %3")
    .arg(cfg_.currentProfile)
    .arg(cfg_.overlayVisible ? "ظاهر" : "مخفي")
    .arg(cfg_.detectionEnabled ? "مفعل" : "متوقف"));

  status_->setText(running_ ? "يعمل" : "متوقف");
  btnRun_->setText(running_ ? "إيقاف" : "بدء");

#ifdef _WIN32
  hotkeys_.registerHotkeys(cfg_.hkToggleRun, cfg_.hkToggleOverlay, cfg_.hkPickColor);
#endif
}

void MainWindow::startEngine() {
  running_ = true;
  frames_ = detections_ = 0;
  avgMs_ = 0.0;
  engineTimer_.start(1000 / std::max(5, cfg_.captureFps));
  Logger::Info("Engine started");
  applyConfigToAll();
}

void MainWindow::stopEngine() {
  running_ = false;
  engineTimer_.stop();
  overlay_->setTargets({}, QRect());
  Logger::Info("Engine stopped");
  applyConfigToAll();
}

QRect MainWindow::computeRoiGlobal() const {
  auto screens = QGuiApplication::screens();
  if (screens.isEmpty()) return QRect();
  int idx = cfg_.selectedScreenIndex;
  if (idx < 0 || idx >= screens.size()) idx = 0;
  QRect sg = screens[idx]->geometry();
  QPoint c = sg.center();
  int r = cfg_.fovRadius;
  QRect roi(c.x() - r, c.y() - r, r*2, r*2);
  roi = roi.intersected(sg);
  return roi;
}

static cv::Mat qimageToBgr(const QImage& img) {
  cv::Mat rgba(img.height(), img.width(), CV_8UC4, (void*)img.bits(), img.bytesPerLine());
  cv::Mat bgr;
  cv::cvtColor(rgba, bgr, cv::COLOR_RGBA2BGR);
  return bgr;
}

void MainWindow::tick() {
  if (!running_ || !cfg_.detectionEnabled) return;

  QElapsedTimer t; t.start();

  QRect roi = computeRoiGlobal();
  if (roi.isEmpty()) return;

  QImage img = capt_.grab(cfg_.selectedScreenIndex, roi);
  if (img.isNull()) return;

  cv::Mat bgr = qimageToBgr(img);
  auto targets = detector_.detect(bgr, cfg_, roi.size());

  auto decision = assist_.onTargets(targets);
  if (cfg_.alertEnabled && decision.requestHint) QApplication::beep();

  overlay_->setTargets(targets, roi);

  frames_++;
  if (!targets.empty()) detections_++;

  double ms = t.elapsed();
  avgMs_ = (avgMs_ * 0.95) + (ms * 0.05);

  pageDash_->setPerfText(QString("FPS الهدف: %1\nزمن إطار تقريبي: %2ms\nإطارات: %3\nاكتشافات: %4")
    .arg(cfg_.captureFps)
    .arg(QString::number(avgMs_, 'f', 1))
    .arg(frames_)
    .arg(detections_));

  pageStats_->setText(QString("إطارات: %1\nاكتشافات: %2\nمتوسط زمن إطار: %3ms\nآخر تحديث: %4")
    .arg(frames_)
    .arg(detections_)
    .arg(QString::number(avgMs_, 'f', 1))
    .arg(QDateTime::currentDateTime().toString("HH:mm:ss")));
}

void MainWindow::pickColorFromScreenCenter() {
  QRect roi = computeRoiGlobal();
  if (roi.isEmpty()) return;
  QImage img = capt_.grab(cfg_.selectedScreenIndex, roi);
  if (img.isNull()) return;

  QColor c = img.pixelColor(QPoint(img.width()/2, img.height()/2));
  auto range = MakeRangeFromColor(c, 12, 60, 60);
  cfg_.enemyRange = range.r; // افتراضي: عدو
  Logger::Info("Picked center pixel -> enemy range updated");
  applyConfigToAll();
  AppConfigIO::Save(cfg_);
}

} // namespace ava
