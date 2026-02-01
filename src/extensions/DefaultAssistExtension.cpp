#include "extensions/DefaultAssistExtension.h"
#include <algorithm>

namespace ava {

AssistDecision DefaultAssistExtension::onTargets(const std::vector<Target>& targets) {
  AssistDecision d;
  for (const auto& t : targets) {
    if (t.confidence >= 0.25f) {
      d.requestHint = true;
      d.strength = std::min(1.f, t.confidence);
      break;
    }
  }
  return d;
}

} // namespace ava
