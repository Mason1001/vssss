#ifdef _WIN32
#include "app/HotkeyManager_win.h"
#include "app/Logger.h"

#include <windows.h>

namespace ava {

static constexpr int HK_ID_TOGGLE_RUN = 0xB00;
static constexpr int HK_ID_TOGGLE_OVERLAY = 0xB01;
static constexpr int HK_ID_PICK_COLOR = 0xB02;

HotkeyManagerWin::HotkeyManagerWin(QObject* parent) : QObject(parent) {}
HotkeyManagerWin::~HotkeyManagerWin() { unregisterHotkeys(); }

bool HotkeyManagerWin::registerHotkeys(int vkToggleRun, int vkToggleOverlay, int vkPickColor) {
  unregisterHotkeys();

  if (!RegisterHotKey(nullptr, HK_ID_TOGGLE_RUN, MOD_NOREPEAT, vkToggleRun)) {
    Logger::Warn("RegisterHotKey failed: toggleRun");
    return false;
  }
  if (!RegisterHotKey(nullptr, HK_ID_TOGGLE_OVERLAY, MOD_NOREPEAT, vkToggleOverlay)) {
    Logger::Warn("RegisterHotKey failed: toggleOverlay");
    UnregisterHotKey(nullptr, HK_ID_TOGGLE_RUN);
    return false;
  }
  if (!RegisterHotKey(nullptr, HK_ID_PICK_COLOR, MOD_NOREPEAT, vkPickColor)) {
    Logger::Warn("RegisterHotKey failed: pickColor");
    UnregisterHotKey(nullptr, HK_ID_TOGGLE_RUN);
    UnregisterHotKey(nullptr, HK_ID_TOGGLE_OVERLAY);
    return false;
  }

  registered_ = true;
  Logger::Info("Global hotkeys registered");
  return true;
}

void HotkeyManagerWin::unregisterHotkeys() {
  if (!registered_) return;
  UnregisterHotKey(nullptr, HK_ID_TOGGLE_RUN);
  UnregisterHotKey(nullptr, HK_ID_TOGGLE_OVERLAY);
  UnregisterHotKey(nullptr, HK_ID_PICK_COLOR);
  registered_ = false;
  Logger::Info("Global hotkeys unregistered");
}

bool HotkeyManagerWin::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
  Q_UNUSED(eventType);
  MSG* msg = static_cast<MSG*>(message);
  if (msg->message == WM_HOTKEY) {
    switch (msg->wParam) {
      case HK_ID_TOGGLE_RUN: emit sigToggleRun(); break;
      case HK_ID_TOGGLE_OVERLAY: emit sigToggleOverlay(); break;
      case HK_ID_PICK_COLOR: emit sigPickColor(); break;
    }
    if (result) *result = 0;
    return true;
  }
  return false;
}

} // namespace ava
#endif
