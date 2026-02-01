# Accessibility Vision Assistant (واجهة عربية)

برنامج Windows 10/11 لمساعدة المستخدمين عبر:
- Overlay فوق الشاشة + FOV (حلقة خارجية + حلقة داخلية)
- التقاط شاشة داخل FOV
- كشف لون/نطاق HSV (عدو/صديق) + Highlight + تنبيه
- منتقي ألوان HSV متقدم (SV + Hue + HEX + RGB/CMYK/HSV/HSL)
- ملفات شخصية (Profiles) + حفظ/تحميل
- أزرار سريعة (Global Hotkeys) + Tray
- Logs + إحصائيات

> **مهم:** هذا المشروع لا يحتوي على أي أتمتة إدخال مرتبطة بأهداف داخل الألعاب (لا تحريك ماوس تلقائي ولا ضغط زر تلقائي).

## المتطلبات
1) Visual Studio 2022 (Desktop development with C++)
2) CMake + Ninja
3) Git
4) Qt 6.10.2 (msvc2022_64) (مع QtSvg)
5) vcpkg (يتم تنزيله تلقائياً عبر scripts)

## بناء سريع (بدون صداع)
- Release: شغّل `scripts/build_release.bat`
- Debug: شغّل `scripts/build_debug.bat`

الناتج:
- `out/build/win-release/AccessibilityVisionAssistant.exe`

## Smoke Test
```
AccessibilityVisionAssistant.exe --smoke-test
```

## أين تُحفظ الإعدادات؟
- داخل `%APPDATA%\AVA\AccessibilityVisionAssistant\`
  - `config.json`
  - `profiles/*.json`
  - `logs/app.log`

## GitHub Actions
ارفع المشروع إلى GitHub، ثم من Actions راح يطلع Artifact عبارة عن ZIP Portable جاهز.
