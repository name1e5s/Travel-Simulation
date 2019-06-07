#ifndef TRANSCONTROLLER_H
#define TRANSCONTROLLER_H

#include <QDebug>
#include <QEasingCurve>
#include <QGeoCoordinate>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#define ANIMATION_DURATION 1000

class TransController : public QObject {
  Q_OBJECT
  Q_PROPERTY(QGeoCoordinate position READ position WRITE setPosition NOTIFY
                 positionChanged)

public:
  TransController();
  ~TransController();
  void setPosition(const QGeoCoordinate &c);
  QGeoCoordinate position() const;

signals:
  void positionChanged();

private:
  QGeoCoordinate currentPosition;
  QEasingCurve easingCurve;
};

#endif // TRANSCONTROLLER_H
