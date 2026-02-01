#include "capture/ScreenCapture.h"
#include "app/Logger.h"

#include <QGuiApplication>
#include <QScreen>

namespace ava {

ScreenCapture::ScreenCapture(QObject* parent) : QObject(parent) {}

QImage ScreenCapture::grab(int screenIndex, const QRect& globalRect) const {
  auto screens = QGuiApplication::screens();
  if (screens.isEmpty()) {
    Logger::Error("No screens found");
    return {};
  }
  if (screenIndex < 0 || screenIndex >= screens.size()) screenIndex = 0;

  QScreen* screen = screens[screenIndex];
  QPixmap pm = screen->grabWindow(0, globalRect.x(), globalRect.y(), globalRect.width(), globalRect.height());
  if (pm.isNull()) {
    Logger::Warn("grabWindow returned null pixmap");
    return {};
  }
  return pm.toImage().convertToFormat(QImage::Format_RGBA8888);
}

} // namespace ava
