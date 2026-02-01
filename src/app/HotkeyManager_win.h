#pragma once
#ifdef _WIN32

#include <QObject>
#include <QAbstractNativeEventFilter>

namespace ava {

class HotkeyManagerWin : public QObject, public QAbstractNativeEventFilter {
  Q_OBJECT
public:
  explicit HotkeyManagerWin(QObject* parent = nullptr);
  ~HotkeyManagerWin() override;

  bool registerHotkeys(int vkToggleRun, int vkToggleOverlay, int vkPickColor);
  void unregisterHotkeys();

  bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

signals:
  void sigToggleRun();
  void sigToggleOverlay();
  void sigPickColor();

private:
  bool registered_ = false;
};

} // namespace ava
#endif
