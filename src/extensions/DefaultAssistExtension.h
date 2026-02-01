#pragma once
#include "extensions/IAssistExtension.h"

namespace ava {

class DefaultAssistExtension : public IAssistExtension {
public:
  AssistDecision onTargets(const std::vector<Target>& targets) override;
};

} // namespace ava
