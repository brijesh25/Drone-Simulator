#include "randomwalkstrategy.h"
#include "dronedata.h"
#include <QtMath>
#include <QRandomGenerator>

RandomWalkStrategy::RandomWalkStrategy()
    : maxStepSize(0.0005)  // Maximum step size in degrees
    , directionChangeChance(0.1)  // 10% chance to change direction each update
    , currentDirection(0.0)
{
    // Initialize with random direction
    currentDirection = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
}

void RandomWalkStrategy::updatePosition(DroneData& drone) {
    // Randomly change direction occasionally
    if (QRandomGenerator::global()->generateDouble() < directionChangeChance) {
        currentDirection = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
    }

    // Calculate step size (random between 0 and maxStepSize)
    double stepSize = QRandomGenerator::global()->generateDouble() * maxStepSize;

    // Calculate new position
    double latStep = stepSize * qCos(currentDirection);
    double lonStep = stepSize * qSin(currentDirection);

    double newLat = drone.getLatitude() + latStep;
    double newLon = drone.getLongitude() + lonStep;

    // Keep within reasonable bounds
    newLat = qBound(28.4, newLat, 29.0);
    newLon = qBound(77.0, newLon, 78.0);

    drone.setLatitude(newLat);
    drone.setLongitude(newLon);

    // Random altitude changes (-5.0 to +5.0)
    double altitudeChange = (QRandomGenerator::global()->generateDouble() - 0.5) * 10.0;
    double newAltitude = qBound(50.0, drone.getAltitude() + altitudeChange, 200.0);
    drone.setAltitude(newAltitude);

    // Update heading and speed
    drone.setHeading(qRadiansToDegrees(currentDirection));
    drone.setSpeed(stepSize * 10000);
}

QString RandomWalkStrategy::getStrategyName() const {
    return "Random Walk";
}
