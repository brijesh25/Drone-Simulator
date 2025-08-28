// Factory Pattern
// Easy to extend with new types

#include "simulationfactory.h"
#include "dronesimulator.h"
#include "hoverstrategy.h"
#include "randomwalkstrategy.h"
#include "movementstrategy.h"
#include "logger.h"

std::unique_ptr<DroneSimulator> SimulationFactory::createSimulator(SimulatorType type) {
    Logger::getInstance().log(Logger::INFO, 
        QString("Creating simulator of type: %1").arg(static_cast<int>(type))); //casting ENUM to integral type

    switch (type) {
        case BASIC_SIMULATOR:
            return std::make_unique<DroneSimulator>();
        case ADVANCED_SIMULATOR:
            // For now, both types create the same simulator
            // In future, these might have different capabilities
            return std::make_unique<DroneSimulator>();
        default:
            Logger::getInstance().log(Logger::ERROR, "Unknown simulator type requested");
            return std::make_unique<DroneSimulator>();
    }
}

std::unique_ptr<MovementStrategy> SimulationFactory::createMovementStrategy(MovementType type) {
    Logger::getInstance().log(Logger::INFO, 
        QString("Creating movement strategy of type: %1").arg(static_cast<int>(type)));

    switch (type) {
        case HOVER_MOVEMENT:
            return std::make_unique<HoverStrategy>();
        case RANDOM_WALK_MOVEMENT:
            return std::make_unique<RandomWalkStrategy>();
        default:
            Logger::getInstance().log(Logger::ERROR, "Unknown movement strategy type requested");
            return std::make_unique<HoverStrategy>();
    }
}
