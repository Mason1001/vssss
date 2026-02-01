#include "vision/ColorMath.h"
#include <algorithm>
#include <cmath>

namespace ava {

static float clamp01(float x) { return std::max(0.f, std::min(1.f, x)); }

HSVf RgbToHsv(const QColor& c) {
  float r = c.redF();
  float g = c.greenF();
  float b = c.blueF();

  float mx = std::max({r,g,b});
  float mn = std::min({r,g,b});
  float d = mx - mn;

  HSVf out;
  out.v = mx;
  out.s = (mx <= 0.f) ? 0.f : (d / mx);

  if (d <= 0.f) {
    out.h = 0.f;
  } else {
    if (mx == r) out.h = 60.f * std::fmod(((g - b) / d), 6.f);
    else if (mx == g) out.h = 60.f * (((b - r) / d) + 2.f);
    else out.h = 60.f * (((r - g) / d) + 4.f);
    if (out.h < 0.f) out.h += 360.f;
  }
  return out;
}

QColor HsvToRgb(const HSVf& hsv) {
  float h = hsv.h;
  float s = clamp01(hsv.s);
  float v = clamp01(hsv.v);

  float c = v * s;
  float x = c * (1.f - std::fabs(std::fmod(h/60.f, 2.f) - 1.f));
  float m = v - c;

  float r=0,g=0,b=0;
  if (h < 60)      { r=c; g=x; b=0; }
  else if (h < 120){ r=x; g=c; b=0; }
  else if (h < 180){ r=0; g=c; b=x; }
  else if (h < 240){ r=0; g=x; b=c; }
  else if (h < 300){ r=x; g=0; b=c; }
  else             { r=c; g=0; b=x; }

  return QColor::fromRgbF(r+m, g+m, b+m);
}

HSLf RgbToHsl(const QColor& c) {
  float r = c.redF();
  float g = c.greenF();
  float b = c.blueF();

  float mx = std::max({r,g,b});
  float mn = std::min({r,g,b});
  float d = mx - mn;

  HSLf out;
  out.l = (mx + mn) * 0.5f;

  if (d <= 0.f) { out.h = 0.f; out.s = 0.f; return out; }

  out.s = d / (1.f - std::fabs(2.f*out.l - 1.f));

  if (mx == r) out.h = 60.f * std::fmod(((g - b)/d), 6.f);
  else if (mx == g) out.h = 60.f * (((b - r)/d) + 2.f);
  else out.h = 60.f * (((r - g)/d) + 4.f);
  if (out.h < 0.f) out.h += 360.f;
  return out;
}

CMYKf RgbToCmyk(const QColor& c) {
  float r = c.redF();
  float g = c.greenF();
  float b = c.blueF();

  CMYKf out;
  out.k = 1.f - std::max({r,g,b});
  if (out.k >= 1.f - 1e-6f) {
    out.c = out.m = out.y = 0.f;
  } else {
    out.c = (1.f - r - out.k) / (1.f - out.k);
    out.m = (1.f - g - out.k) / (1.f - out.k);
    out.y = (1.f - b - out.k) / (1.f - out.k);
  }
  out.c = clamp01(out.c); out.m = clamp01(out.m); out.y = clamp01(out.y); out.k = clamp01(out.k);
  return out;
}

QString ToHex(const QColor& c) {
  return QString("#%1%2%3")
      .arg(c.red(),   2, 16, QLatin1Char('0'))
      .arg(c.green(), 2, 16, QLatin1Char('0'))
      .arg(c.blue(),  2, 16, QLatin1Char('0'))
      .toUpper();
}

} // namespace ava
