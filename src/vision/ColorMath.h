#pragma once
#include <QColor>
#include <QString>

namespace ava {

struct HSVf { float h=0.f, s=0.f, v=0.f; };   // h:0..360, s/v:0..1
struct HSLf { float h=0.f, s=0.f, l=0.f; };   // h:0..360, s/l:0..1
struct CMYKf { float c=0.f, m=0.f, y=0.f, k=0.f; }; // 0..1

HSVf RgbToHsv(const QColor& c);
QColor HsvToRgb(const HSVf& hsv);

HSLf RgbToHsl(const QColor& c);
CMYKf RgbToCmyk(const QColor& c);

QString ToHex(const QColor& c);

} // namespace ava
