#ifndef TRANSCONTROLLER_H
#define TRANSCONTROLLER_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QDebug>
#include <QEasingCurve>
#include <QGeoCoordinate>

#define ANIMATION_DURATION 1000

class TransController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate position READ position WRITE setPosition NOTIFY positionChanged)

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
