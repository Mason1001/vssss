#include "overlay/OverlayWindow.h"

#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace ava {

OverlayWindow::OverlayWindow(QWidget* parent) : QWidget(parent) {
  setWindowFlag(Qt::FramelessWindowHint, true);
  setWindowFlag(Qt::Tool, true);
  setWindowFlag(Qt::WindowStaysOnTopHint, true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_ShowWithoutActivating, true);

  QRect virtualGeom;
  for (auto s : QGuiApplication::screens()) virtualGeom = virtualGeom.united(s->geometry());
  setGeometry(virtualGeom);
}

void OverlayWindow::applyConfig(const AppConfig& cfg) {
  cfg_ = cfg;
  setVisible(cfg_.overlayVisible && cfg_.overlayEnabled);
#ifdef _WIN32
  applyClickThroughWin(cfg_.overlayClickThrough);
#endif
  update();
}

void OverlayWindow::setTargets(const std::vector<Target>& targets, const QRect& roiGlobalRect) {
  targets_ = targets;
  roiGlobal_ = roiGlobalRect;
  update();
}

void OverlayWindow::setRunning(bool running) {
  running_ = running;
  update();
}

void OverlayWindow::showEvent(QShowEvent* e) {
  QWidget::showEvent(e);
#ifdef _WIN32
  HWND hwnd = (HWND)winId();
  MARGINS margins = { -1 };
  DwmExtendFrameIntoClientArea(hwnd, &margins);
  applyClickThroughWin(cfg_.overlayClickThrough);
#endif
}

void OverlayWindow::applyClickThroughWin(bool enable) {
#ifdef _WIN32
  HWND hwnd = (HWND)winId();
  LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  exStyle |= WS_EX_LAYERED;
  if (enable) exStyle |= WS_EX_TRANSPARENT;
  else exStyle &= ~WS_EX_TRANSPARENT;
  SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
#endif
}

void OverlayWindow::paintEvent(QPaintEvent*) {
  if (!cfg_.overlayEnabled || !cfg_.overlayVisible) return;

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, true);
  p.setOpacity(std::max(0.15, std::min(1.0, cfg_.overlayOpacity / 255.0)));

  auto screens = QGuiApplication::screens();
  if (screens.isEmpty()) return;
  int idx = cfg_.selectedScreenIndex;
  if (idx < 0 || idx >= screens.size()) idx = 0;
  QRect sg = screens[idx]->geometry();
  QPoint center = sg.center();

  // Outer FOV
  QPen pen(QColor("#2D7DFF"));
  pen.setWidth(std::max(1, cfg_.fovThickness));
  p.setPen(pen);
  p.setBrush(Qt::NoBrush);
  p.drawEllipse(center, cfg_.fovRadius, cfg_.fovRadius);

  // Inner zone
  QPen pen2(QColor("#2D7DFF"));
  pen2.setStyle(Qt::DashLine);
  pen2.setWidth(2);
  p.setPen(pen2);
  p.drawEllipse(center, cfg_.fovInnerRadius, cfg_.fovInnerRadius);

  // Status
  p.setOpacity(1.0);
  QFont f = p.font();
  f.setPointSize(10);
  p.setFont(f);
  p.setPen(Qt::white);
  QString status = running_ ? "يعمل" : "متوقف";
  p.drawText(QRect(sg.topLeft() + QPoint(12,12), QSize(180, 22)),
             Qt::AlignLeft|Qt::AlignVCenter, QString("الحالة: %1").arg(status));

  if (!cfg_.highlightEnabled) return;

  p.setOpacity(std::max(0.25, std::min(1.0, cfg_.overlayOpacity / 255.0)));

  for (const auto& t : targets_) {
    QRect gr(roiGlobal_.x() + t.bbox.x(), roiGlobal_.y() + t.bbox.y(), t.bbox.width(), t.bbox.height());
    QColor c = (t.kind == TargetKind::Enemy) ? QColor(255, 60, 60) : QColor(60, 255, 140);

    QPen tp(c);
    tp.setWidth(2);
    p.setPen(tp);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(gr.adjusted(-2,-2,2,2), 6, 6);

    QColor halo = c; halo.setAlpha(70);
    p.setBrush(halo);
    p.setPen(Qt::NoPen);
    p.drawEllipse(gr.center(), std::max(10, gr.width()/2), std::max(10, gr.height()/2));

    p.setPen(Qt::white);
    p.setBrush(Qt::NoBrush);
    p.drawText(gr.adjusted(0, -18, 0, 0), Qt::AlignLeft|Qt::AlignTop,
               QString("%1%").arg(int(t.confidence * 100)));
  }
}

} // namespace ava
