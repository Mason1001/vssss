#include "app/Logger.h"

#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <QStandardPaths>
#include <QDir>

namespace ava {

static std::shared_ptr<spdlog::logger> g_logger;

static std::string toStdString(const QString& s) {
  auto u8 = s.toUtf8();
  return std::string(u8.constData(), u8.size());
}

void Logger::Init() {
  if (g_logger) return;

  const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(appData + "/logs");
  const QString logPath = appData + "/logs/app.log";

  auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(toStdString(logPath), 5 * 1024 * 1024, 3);
#ifdef _MSC_VER
  auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
  std::vector<spdlog::sink_ptr> sinks { file_sink, msvc_sink };
#else
  std::vector<spdlog::sink_ptr> sinks { file_sink };
#endif

  g_logger = std::make_shared<spdlog::logger>("ava", sinks.begin(), sinks.end());
  g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
  g_logger->set_level(spdlog::level::info);
  spdlog::set_default_logger(g_logger);

  Info("Logger initialized");
}

void Logger::Shutdown() {
  if (!g_logger) return;
  Info("Logger shutdown");
  g_logger.reset();
  spdlog::shutdown();
}

void Logger::Info(const std::string& msg)  { if (g_logger) g_logger->info(msg); }
void Logger::Warn(const std::string& msg)  { if (g_logger) g_logger->warn(msg); }
void Logger::Error(const std::string& msg) { if (g_logger) g_logger->error(msg); }

} // namespace ava
