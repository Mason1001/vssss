#pragma once
#include <QString>

namespace ava {

struct ColorRangeHSV {
  int hMin = 0;
  int hMax = 360;
  int sMin = 0;
  int sMax = 255;
  int vMin = 0;
  int vMax = 255;
};

struct AppConfig {
  // Overlay
  bool overlayEnabled = true;
  bool overlayVisible = true;
  bool overlayClickThrough = true;
  int overlayOpacity = 180; // 0..255
  int fovRadius = 220;
  int fovInnerRadius = 110;
  int fovThickness = 3;

  // Capture
  int captureFps = 30;
  int selectedScreenIndex = 0;

  // Detection
  bool detectionEnabled = true;
  bool highlightEnabled = true;
  bool alertEnabled = true;

  // Enemy/Friend ranges
  ColorRangeHSV enemyRange;
  ColorRangeHSV friendRange;
  int minContourArea = 60;
  int morphKernel = 3; // odd recommended

  // UI
  QString currentProfile = "افتراضي";

  // Hotkeys (virtual-key codes)
  int hkToggleRun = 0x77;     // F8
  int hkToggleOverlay = 0x78; // F9
  int hkPickColor = 0x79;     // F10
};

class AppConfigIO {
public:
  static AppConfig Load();
  static void Save(const AppConfig& cfg);
  static QString AppDataDir();
  static QString ConfigPath();
};

} // namespace ava
