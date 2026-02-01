#pragma once
#include <QWidget>
#include "app/ProfileManager.h"
#include "app/AppConfig.h"

class QListWidget;
class QLineEdit;
class QPushButton;

namespace ava {

class ProfilesPage : public QWidget {
  Q_OBJECT
public:
  explicit ProfilesPage(ProfileManager* pm, QWidget* parent=nullptr);

  void setConfig(const AppConfig& cfg);
  void refreshList();

signals:
  void requestLoadProfile(const QString& name);
  void requestSaveProfile(const QString& name);
  void requestDeleteProfile(const QString& name);

private:
  ProfileManager* pm_ = nullptr;
  AppConfig cfg_;
  QListWidget* list_ = nullptr;
  QLineEdit* name_ = nullptr;
  QPushButton* btnSave_ = nullptr;
  QPushButton* btnLoad_ = nullptr;
  QPushButton* btnDelete_ = nullptr;
};

} // namespace ava
