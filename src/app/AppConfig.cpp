#include "app/AppConfig.h"
#include "app/Logger.h"

#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include <nlohmann/json.hpp>

namespace ava {

QString AppConfigIO::AppDataDir() {
  return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString AppConfigIO::ConfigPath() {
  return AppDataDir() + "/config.json";
}

static void fromJsonRange(const nlohmann::json& j, ColorRangeHSV& r) {
  r.hMin = j.value("hMin", r.hMin);
  r.hMax = j.value("hMax", r.hMax);
  r.sMin = j.value("sMin", r.sMin);
  r.sMax = j.value("sMax", r.sMax);
  r.vMin = j.value("vMin", r.vMin);
  r.vMax = j.value("vMax", r.vMax);
}

static nlohmann::json toJsonRange(const ColorRangeHSV& r) {
  return {{"hMin",r.hMin},{"hMax",r.hMax},{"sMin",r.sMin},{"sMax",r.sMax},{"vMin",r.vMin},{"vMax",r.vMax}};
}

AppConfig AppConfigIO::Load() {
  AppConfig cfg;
  QDir().mkpath(AppDataDir());

  QFile f(ConfigPath());
  if (!f.exists()) {
    Logger::Warn("config.json not found; using defaults");
    return cfg;
  }
  if (!f.open(QIODevice::ReadOnly)) {
    Logger::Error("Failed to open config.json; using defaults");
    return cfg;
  }

  auto bytes = f.readAll();
  f.close();

  try {
    auto j = nlohmann::json::parse(bytes.constData());

    cfg.overlayEnabled = j.value("overlayEnabled", cfg.overlayEnabled);
    cfg.overlayVisible = j.value("overlayVisible", cfg.overlayVisible);
    cfg.overlayClickThrough = j.value("overlayClickThrough", cfg.overlayClickThrough);
    cfg.overlayOpacity = j.value("overlayOpacity", cfg.overlayOpacity);
    cfg.fovRadius = j.value("fovRadius", cfg.fovRadius);
    cfg.fovInnerRadius = j.value("fovInnerRadius", cfg.fovInnerRadius);
    cfg.fovThickness = j.value("fovThickness", cfg.fovThickness);

    cfg.captureFps = j.value("captureFps", cfg.captureFps);
    cfg.selectedScreenIndex = j.value("selectedScreenIndex", cfg.selectedScreenIndex);

    cfg.detectionEnabled = j.value("detectionEnabled", cfg.detectionEnabled);
    cfg.highlightEnabled = j.value("highlightEnabled", cfg.highlightEnabled);
    cfg.alertEnabled = j.value("alertEnabled", cfg.alertEnabled);

    if (j.contains("enemyRange")) fromJsonRange(j["enemyRange"], cfg.enemyRange);
    if (j.contains("friendRange")) fromJsonRange(j["friendRange"], cfg.friendRange);

    cfg.minContourArea = j.value("minContourArea", cfg.minContourArea);
    cfg.morphKernel = j.value("morphKernel", cfg.morphKernel);

    cfg.currentProfile = QString::fromUtf8(j.value("currentProfile", cfg.currentProfile.toUtf8().constData()).c_str());

    cfg.hkToggleRun = j.value("hkToggleRun", cfg.hkToggleRun);
    cfg.hkToggleOverlay = j.value("hkToggleOverlay", cfg.hkToggleOverlay);
    cfg.hkPickColor = j.value("hkPickColor", cfg.hkPickColor);

    Logger::Info("Loaded config.json");
  } catch (...) {
    Logger::Error("Failed parsing config.json; using defaults");
  }
  return cfg;
}

void AppConfigIO::Save(const AppConfig& cfg) {
  QDir().mkpath(AppDataDir());

  nlohmann::json j;
  j["overlayEnabled"] = cfg.overlayEnabled;
  j["overlayVisible"] = cfg.overlayVisible;
  j["overlayClickThrough"] = cfg.overlayClickThrough;
  j["overlayOpacity"] = cfg.overlayOpacity;
  j["fovRadius"] = cfg.fovRadius;
  j["fovInnerRadius"] = cfg.fovInnerRadius;
  j["fovThickness"] = cfg.fovThickness;

  j["captureFps"] = cfg.captureFps;
  j["selectedScreenIndex"] = cfg.selectedScreenIndex;

  j["detectionEnabled"] = cfg.detectionEnabled;
  j["highlightEnabled"] = cfg.highlightEnabled;
  j["alertEnabled"] = cfg.alertEnabled;

  j["enemyRange"] = toJsonRange(cfg.enemyRange);
  j["friendRange"] = toJsonRange(cfg.friendRange);

  j["minContourArea"] = cfg.minContourArea;
  j["morphKernel"] = cfg.morphKernel;

  j["currentProfile"] = cfg.currentProfile.toUtf8().constData();

  j["hkToggleRun"] = cfg.hkToggleRun;
  j["hkToggleOverlay"] = cfg.hkToggleOverlay;
  j["hkPickColor"] = cfg.hkPickColor;

  QFile f(ConfigPath());
  if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    Logger::Error("Failed to write config.json");
    return;
  }
  f.write(QByteArray::fromStdString(j.dump(2)));
  f.close();
  Logger::Info("Saved config.json");
}

} // namespace ava
