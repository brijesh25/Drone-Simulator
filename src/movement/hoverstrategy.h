#ifndef HOVERSTRATEGY_H
#define HOVERSTRATEGY_H

#include "movementstrategy.h"
#include <QString>

class HoverStrategy : public MovementStrategy {
public:
    HoverStrategy();
    void updatePosition(DroneData& drone) override;
    QString getStrategyName() const override;

private:
    double hoverRadius;
    double centerLat;
    double centerLon;
    double angle;
};

#endif // HOVERSTRATEGY_H