#include "vision/ColorRange.h"
#include "vision/ColorMath.h"
#include <algorithm>
#include <cmath>

namespace ava {

static int clampi(int v, int lo, int hi) { return std::max(lo, std::min(hi, v)); }

ColorRange MakeRangeFromColor(const QColor& c, int hueTolDeg, int satTol, int valTol) {
  auto hsv = RgbToHsv(c);

  int h = (int)std::round(hsv.h);
  int s = (int)std::round(hsv.s * 255.f);
  int v = (int)std::round(hsv.v * 255.f);

  ColorRange out;
  out.r.hMin = h - hueTolDeg;
  out.r.hMax = h + hueTolDeg;

  if (out.r.hMin < 0) out.r.hMin += 360;
  if (out.r.hMax >= 360) out.r.hMax -= 360;

  out.r.sMin = clampi(s - satTol, 0, 255);
  out.r.sMax = clampi(s + satTol, 0, 255);
  out.r.vMin = clampi(v - valTol, 0, 255);
  out.r.vMax = clampi(v + valTol, 0, 255);
  return out;
}

} // namespace ava
