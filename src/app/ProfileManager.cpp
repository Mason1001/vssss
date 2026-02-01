#include "app/ProfileManager.h"
#include "app/Logger.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include <nlohmann/json.hpp>

namespace ava {

static QString safeName(QString s) {
  s = s.trimmed();
  s.replace("/", "_").replace("\\", "_").replace("..", "_");
  if (s.isEmpty()) s = "افتراضي";
  return s;
}

ProfileManager::ProfileManager() {
  dir_ = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/profiles";
  QDir().mkpath(dir_);
}

QString ProfileManager::profilesDir() const { return dir_; }

QStringList ProfileManager::listProfiles() const {
  QDir d(dir_);
  auto files = d.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);
  QStringList out;
  for (auto& f : files) out << QFileInfo(f).baseName();
  if (!out.contains("افتراضي")) out.prepend("افتراضي");
  return out;
}

bool ProfileManager::saveProfile(const QString& name, const AppConfig& cfg) {
  const QString n = safeName(name);
  const QString path = dir_ + "/" + n + ".json";

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

  j["enemyRange"] = {{"hMin",cfg.enemyRange.hMin},{"hMax",cfg.enemyRange.hMax},{"sMin",cfg.enemyRange.sMin},{"sMax",cfg.enemyRange.sMax},{"vMin",cfg.enemyRange.vMin},{"vMax",cfg.enemyRange.vMax}};
  j["friendRange"] = {{"hMin",cfg.friendRange.hMin},{"hMax",cfg.friendRange.hMax},{"sMin",cfg.friendRange.sMin},{"sMax",cfg.friendRange.sMax},{"vMin",cfg.friendRange.vMin},{"vMax",cfg.friendRange.vMax}};

  j["minContourArea"] = cfg.minContourArea;
  j["morphKernel"] = cfg.morphKernel;

  j["currentProfile"] = n.toUtf8().constData();

  j["hkToggleRun"] = cfg.hkToggleRun;
  j["hkToggleOverlay"] = cfg.hkToggleOverlay;
  j["hkPickColor"] = cfg.hkPickColor;

  QFile f(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    Logger::Error("Failed saving profile");
    return false;
  }
  f.write(QByteArray::fromStdString(j.dump(2)));
  f.close();
  Logger::Info("Saved profile: " + n.toStdString());
  return true;
}

bool ProfileManager::loadProfile(const QString& name, AppConfig& outCfg) {
  const QString n = safeName(name);
  const QString path = dir_ + "/" + n + ".json";
  QFile f(path);
  if (!f.exists()) return false;
  if (!f.open(QIODevice::ReadOnly)) return false;
  auto bytes = f.readAll();
  f.close();

  try {
    auto j = nlohmann::json::parse(bytes.constData());
    outCfg = AppConfig{};
    outCfg.overlayEnabled = j.value("overlayEnabled", outCfg.overlayEnabled);
    outCfg.overlayVisible = j.value("overlayVisible", outCfg.overlayVisible);
    outCfg.overlayClickThrough = j.value("overlayClickThrough", outCfg.overlayClickThrough);
    outCfg.overlayOpacity = j.value("overlayOpacity", outCfg.overlayOpacity);
    outCfg.fovRadius = j.value("fovRadius", outCfg.fovRadius);
    outCfg.fovInnerRadius = j.value("fovInnerRadius", outCfg.fovInnerRadius);
    outCfg.fovThickness = j.value("fovThickness", outCfg.fovThickness);

    outCfg.captureFps = j.value("captureFps", outCfg.captureFps);
    outCfg.selectedScreenIndex = j.value("selectedScreenIndex", outCfg.selectedScreenIndex);

    outCfg.detectionEnabled = j.value("detectionEnabled", outCfg.detectionEnabled);
    outCfg.highlightEnabled = j.value("highlightEnabled", outCfg.highlightEnabled);
    outCfg.alertEnabled = j.value("alertEnabled", outCfg.alertEnabled);

    auto er = j.value("enemyRange", nlohmann::json::object());
    outCfg.enemyRange.hMin = er.value("hMin", outCfg.enemyRange.hMin);
    outCfg.enemyRange.hMax = er.value("hMax", outCfg.enemyRange.hMax);
    outCfg.enemyRange.sMin = er.value("sMin", outCfg.enemyRange.sMin);
    outCfg.enemyRange.sMax = er.value("sMax", outCfg.enemyRange.sMax);
    outCfg.enemyRange.vMin = er.value("vMin", outCfg.enemyRange.vMin);
    outCfg.enemyRange.vMax = er.value("vMax", outCfg.enemyRange.vMax);

    auto fr = j.value("friendRange", nlohmann::json::object());
    outCfg.friendRange.hMin = fr.value("hMin", outCfg.friendRange.hMin);
    outCfg.friendRange.hMax = fr.value("hMax", outCfg.friendRange.hMax);
    outCfg.friendRange.sMin = fr.value("sMin", outCfg.friendRange.sMin);
    outCfg.friendRange.sMax = fr.value("sMax", outCfg.friendRange.sMax);
    outCfg.friendRange.vMin = fr.value("vMin", outCfg.friendRange.vMin);
    outCfg.friendRange.vMax = fr.value("vMax", outCfg.friendRange.vMax);

    outCfg.minContourArea = j.value("minContourArea", outCfg.minContourArea);
    outCfg.morphKernel = j.value("morphKernel", outCfg.morphKernel);

    outCfg.currentProfile = n;

    outCfg.hkToggleRun = j.value("hkToggleRun", outCfg.hkToggleRun);
    outCfg.hkToggleOverlay = j.value("hkToggleOverlay", outCfg.hkToggleOverlay);
    outCfg.hkPickColor = j.value("hkPickColor", outCfg.hkPickColor);

    Logger::Info("Loaded profile: " + n.toStdString());
    return true;
  } catch (...) {
    Logger::Error("Failed parsing profile");
    return false;
  }
}

bool ProfileManager::deleteProfile(const QString& name) {
  const QString n = safeName(name);
  if (n == "افتراضي") return false;
  const QString path = dir_ + "/" + n + ".json";
  return QFile::remove(path);
}

} // namespace ava
