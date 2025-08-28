#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <QString>

class DroneData;

// Strategy Pattern Implementation
class MovementStrategy {
public:
    virtual ~MovementStrategy() = default;
    virtual void updatePosition(DroneData& drone) = 0;
    virtual QString getStrategyName() const = 0;
};

#endif // MOVEMENTSTRATEGY_H
