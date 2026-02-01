#pragma once
#include "app/AppConfig.h"
#include <QStringList>

namespace ava {

class ProfileManager {
public:
  ProfileManager();

  QString profilesDir() const;
  QStringList listProfiles() const;

  bool saveProfile(const QString& name, const AppConfig& cfg);
  bool loadProfile(const QString& name, AppConfig& outCfg);
  bool deleteProfile(const QString& name);

private:
  QString dir_;
};

} // namespace ava
