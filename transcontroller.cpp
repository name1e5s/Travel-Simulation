#include "transcontroller.h"

TransController::TransController()
{
    easingCurve.setType(QEasingCurve::InOutQuad);
    easingCurve.setPeriod(ANIMATION_DURATION);
}

TransController::~TransController()
{

}

void TransController::setPosition(const QGeoCoordinate &c)
{
    if (currentPosition == c)
            return;

        currentPosition = c;
        emit positionChanged();
}

QGeoCoordinate TransController::position() const
{
    return currentPosition;
}
