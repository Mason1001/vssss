#include <QApplication>
#include <QCommandLineParser>

#include "app/Logger.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("AccessibilityVisionAssistant");
  app.setOrganizationName("AVA");

  ava::Logger::Init();

  QCommandLineParser parser;
  parser.setApplicationDescription("Accessibility Vision Assistant");
  parser.addHelpOption();
  QCommandLineOption smoke("smoke-test", "Run quick startup and exit 0.");
  parser.addOption(smoke);
  parser.process(app);

  if (parser.isSet(smoke)) {
    ava::Logger::Info("Smoke test OK");
    ava::Logger::Shutdown();
    return 0;
  }

  ava::MainWindow w;
  w.show();

  int rc = app.exec();
  ava::Logger::Shutdown();
  return rc;
}
