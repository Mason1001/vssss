#pragma once
#include <QObject>
#include <QImage>
#include <QRect>

namespace ava {

class ScreenCapture : public QObject {
  Q_OBJECT
public:
  explicit ScreenCapture(QObject* parent = nullptr);

  // Capture ROI rect in global coordinates on a given screen index
  QImage grab(int screenIndex, const QRect& globalRect) const;
};

} // namespace ava
