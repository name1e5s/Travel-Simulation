#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQuickStyle::setStyle("Material");
  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/ui/ui/MainWindow.qml")));

  return app.exec();
}
