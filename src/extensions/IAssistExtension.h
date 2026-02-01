#pragma once
#include <vector>
#include "vision/Detector.h"

namespace ava {

// واجهة امتداد عامة للمساعدة (بدون أي إدخال).
struct AssistDecision {
  bool requestHint = false;
  float strength = 0.f; // 0..1
};

class IAssistExtension {
public:
  virtual ~IAssistExtension() = default;
  virtual AssistDecision onTargets(const std::vector<Target>& targets) = 0;
};

} // namespace ava
