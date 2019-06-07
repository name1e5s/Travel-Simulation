#include "transcontroller.h"

/**
 * @brief Construct a new Trans Controller:: Trans Controller object
 *
 */
TransController::TransController() {
  easingCurve.setType(QEasingCurve::InOutQuad);
  easingCurve.setPeriod(ANIMATION_DURATION);
}

/**
 * @brief Destroy the Trans Controller:: Trans Controller object
 *
 */
TransController::~TransController() {}

void TransController::setPosition(const QGeoCoordinate &c) {
  if (currentPosition == c)
    return;

  currentPosition = c;
  emit positionChanged();
}

QGeoCoordinate TransController::position() const { return currentPosition; }
