#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickStyle>
#include <QTextCodec>
#include <graph_handler.h>
#include <transcontroller.h>
#include <iostream>

/**
 * @brief Entry point of the whole app.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QQuickStyle::setStyle("Material");

  TransController transController;

  engine.rootContext()->setContextProperty("transController", &transController);
  qmlRegisterType<GraphHandler>("com.name1e5s.travel", 1, 0, "GraphHandler");
  qRegisterMetaType<std::vector<int>>();

  engine.load(QUrl(QStringLiteral("qrc:/ui/ui/MainWindow.qml")));

  return app.exec();
}
