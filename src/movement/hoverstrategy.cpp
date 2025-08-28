#include "hoverstrategy.h"
#include "dronedata.h"
#include <QtMath>
#include <QRandomGenerator>

HoverStrategy::HoverStrategy()
    : hoverRadius(0.001)  // Small radius for hovering
    , centerLat(28.4595)
    , centerLon(77.0266)
    , angle(0.0)
{
}

void HoverStrategy::updatePosition(DroneData& drone) {
    // Create small circular movement to simulate hovering with minor drift
    angle += 0.1;  // Slow rotation
    if (angle >= 2 * M_PI) {
        angle = 0.0;
    }

    // Add small random variations
    double randomFactor = 0.5 + QRandomGenerator::global()->generateDouble();
    double currentRadius = hoverRadius * randomFactor;

    double newLat = centerLat + currentRadius * qCos(angle);
    double newLon = centerLon + currentRadius * qSin(angle);

    drone.setLatitude(newLat);
    drone.setLongitude(newLon);

    // Slight altitude variation
    double altVariation = -2.0 + QRandomGenerator::global()->generateDouble();
    drone.setAltitude(100.0 + altVariation);

    // Update heading to face movement direction
    drone.setHeading(qRadiansToDegrees(angle));

    // Low speed for hovering
    drone.setSpeed(0.5 + QRandomGenerator::global()->generateDouble());
}

QString HoverStrategy::getStrategyName() const {
    return "Hover Mode";
}
