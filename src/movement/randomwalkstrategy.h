#ifndef RANDOMWALKSTRATEGY_H
#define RANDOMWALKSTRATEGY_H

#include "movementstrategy.h"
#include <QString>

class RandomWalkStrategy : public MovementStrategy {
public:
    RandomWalkStrategy();
    void updatePosition(DroneData& drone) override;
    QString getStrategyName() const override;

private:
    double maxStepSize;
    double directionChangeChance;
    double currentDirection;
};

#endif // RANDOMWALKSTRATEGY_H