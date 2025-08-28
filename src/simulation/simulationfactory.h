#ifndef SIMULATIONFACTORY_H
#define SIMULATIONFACTORY_H

#include <memory>

class DroneSimulator;
class MovementStrategy;

// Factory Pattern Implementation
class SimulationFactory {
public:
    enum SimulatorType {
        BASIC_SIMULATOR,
        ADVANCED_SIMULATOR
    };

    enum MovementType {
        HOVER_MOVEMENT,
        RANDOM_WALK_MOVEMENT
    };

    static std::unique_ptr<DroneSimulator> createSimulator(SimulatorType type);
    static std::unique_ptr<MovementStrategy> createMovementStrategy(MovementType type);
};

#endif // SIMULATIONFACTORY_H