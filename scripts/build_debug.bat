@echo off
setlocal
chcp 65001 >nul

set ROOT=%~dp0..
set VCPKG=%ROOT%\vcpkg

if not exist "%VCPKG%\vcpkg.exe" (
  echo [INFO] vcpkg غير موجود. جاري تنزيله...
  git clone https://github.com/microsoft/vcpkg "%VCPKG%" || exit /b 1
)

call "%VCPKG%\bootstrap-vcpkg.bat" || exit /b 1

echo [INFO] تثبيت التبعيات من vcpkg.json...
"%VCPKG%\vcpkg.exe" install --triplet x64-windows || exit /b 1

echo [INFO] بناء Debug...
cmake --preset win-debug || exit /b 1
cmake --build --preset win-debug || exit /b 1

echo [DONE] تم البناء. ستجد الملف هنا:
echo %ROOT%\out\build\win-debug\AccessibilityVisionAssistant.exe
pause
