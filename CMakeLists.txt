cmake_minimum_required(VERSION 3.21)

project(AccessibilityVisionAssistant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Svg)

# OpenCV
find_package(OpenCV REQUIRED)

# Third-party (vcpkg)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(AccessibilityVisionAssistant
  resources/app.qrc

  src/main.cpp

  src/app/AppConfig.h
  src/app/AppConfig.cpp
  src/app/Logger.h
  src/app/Logger.cpp
  src/app/ProfileManager.h
  src/app/ProfileManager.cpp
  src/app/HotkeyManager_win.h
  src/app/HotkeyManager_win.cpp

  src/capture/ScreenCapture.h
  src/capture/ScreenCapture.cpp

  src/vision/ColorMath.h
  src/vision/ColorMath.cpp
  src/vision/ColorRange.h
  src/vision/ColorRange.cpp
  src/vision/Detector.h
  src/vision/Detector.cpp

  src/overlay/OverlayWindow.h
  src/overlay/OverlayWindow.cpp

  src/extensions/IAssistExtension.h
  src/extensions/DefaultAssistExtension.h
  src/extensions/DefaultAssistExtension.cpp

  src/ui/MainWindow.h
  src/ui/MainWindow.cpp

  src/ui/pages/DashboardPage.h
  src/ui/pages/DashboardPage.cpp
  src/ui/pages/OverlayPage.h
  src/ui/pages/OverlayPage.cpp
  src/ui/pages/ColorsPage.h
  src/ui/pages/ColorsPage.cpp
  src/ui/pages/DetectionPage.h
  src/ui/pages/DetectionPage.cpp
  src/ui/pages/ProfilesPage.h
  src/ui/pages/ProfilesPage.cpp
  src/ui/pages/HotkeysPage.h
  src/ui/pages/HotkeysPage.cpp
  src/ui/pages/StatsPage.h
  src/ui/pages/StatsPage.cpp
  src/ui/pages/SystemPage.h
  src/ui/pages/SystemPage.cpp

  src/ui/widgets/ColorPickerPanel.h
  src/ui/widgets/ColorPickerPanel.cpp
  src/ui/widgets/SVPicker.h
  src/ui/widgets/SVPicker.cpp
  src/ui/widgets/HueSlider.h
  src/ui/widgets/HueSlider.cpp
  src/ui/widgets/ValueCard.h
  src/ui/widgets/ValueCard.cpp
)

target_include_directories(AccessibilityVisionAssistant PRIVATE src)

target_link_libraries(AccessibilityVisionAssistant PRIVATE
  Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg
  ${OpenCV_LIBS}
  spdlog::spdlog
  nlohmann_json::nlohmann_json
)

if (WIN32)
  target_compile_definitions(AccessibilityVisionAssistant PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()
