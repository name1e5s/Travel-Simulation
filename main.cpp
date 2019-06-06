﻿#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickStyle>
#include <QTextCodec>
#include <graph_handler.h>
#include <transcontroller.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  CityGraph cityGraph;
  QQuickStyle::setStyle("Material");

  TransController transController;

  QQmlApplicationEngine engine;

  engine.rootContext()->setContextProperty("transController", &transController);
  qmlRegisterType<GraphHandler>("com.name1e5s.travel", 1, 0, "GraphHandler");
  qRegisterMetaType<std::vector<int>>();
  engine.load(QUrl(QStringLiteral("qrc:/ui/ui/MainWindow.qml")));

  return app.exec();
}
