#include "ui/pages/HotkeysPage.h"
#include <QVBoxLayout>
#include <QLabel>

namespace ava {

HotkeysPage::HotkeysPage(QWidget* parent) : QWidget(parent) {
  setLayoutDirection(Qt::RightToLeft);
  auto* root = new QVBoxLayout(this);
  root->setContentsMargins(16,16,16,16);
  root->setSpacing(10);

  auto* t = new QLabel("الأزرار السريعة (Global Hotkeys)", this);
  t->setObjectName("PageTitle");

  info_ = new QLabel(this);
  info_->setWordWrap(true);

  root->addWidget(t);
  root->addWidget(info_);
  root->addStretch(1);
}

static QString vkName(int vk) {
  if (vk == 0x77) return "F8";
  if (vk == 0x78) return "F9";
  if (vk == 0x79) return "F10";
  return QString("VK_%1").arg(vk);
}

void HotkeysPage::setConfig(const AppConfig& cfg) {
  QString s;
  s += QString("تبديل التشغيل/الإيقاف: %1\n").arg(vkName(cfg.hkToggleRun));
  s += QString("إظهار/إخفاء Overlay: %1\n").arg(vkName(cfg.hkToggleOverlay));
  s += QString("التقاط لون من الشاشة: %1\n").arg(vkName(cfg.hkPickColor));
  s += "\n\n(يمكن تعديلها لاحقاً.)";
  info_->setText(s);
}

} // namespace ava
