#pragma once
#include "app/AppConfig.h"
#include <QColor>

namespace ava {

struct ColorRange {
  ColorRangeHSV r;
  bool isHueWrap() const { return r.hMin > r.hMax; }
};

ColorRange MakeRangeFromColor(const QColor& c, int hueTolDeg, int satTol, int valTol);

} // namespace ava
