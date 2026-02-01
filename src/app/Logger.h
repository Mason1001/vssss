#pragma once
#include <string>

namespace ava {

class Logger {
public:
  static void Init();
  static void Shutdown();
  static void Info(const std::string& msg);
  static void Warn(const std::string& msg);
  static void Error(const std::string& msg);
};

} // namespace ava
