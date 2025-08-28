#include "dronesimulator.h"
#include "movementstrategy.h"
#include "logger.h"
#include <QRandomGenerator>
#include <algorithm>

DroneSimulator::DroneSimulator(QObject *parent)
    : QObject(parent)
    , updateTimer(new QTimer(this))
    , isSimulationRunning(false)
    , failureMode(false)
    , updateCount(0)
    , batteryDrainRate(0.1)
{
    initializeDrone();

    // Set up timer for 500ms updates as required
    updateTimer->setInterval(500);
    connect(updateTimer, &QTimer::timeout, this, &DroneSimulator::updateTelemetry);

    Logger::getInstance().log(Logger::INFO, 
        QString("DroneSimulator initialized for drone: %1").arg(droneData.getId()));
}

DroneSimulator::~DroneSimulator() {
    stopSimulation();
    observers.clear();
    Logger::getInstance().log(Logger::INFO, "DroneSimulator destroyed");
}

void DroneSimulator::attach(Observer* observer) {
    if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
        observers.push_back(observer);
        Logger::getInstance().log(Logger::DEBUG, "Observer attached to DroneSimulator");
    }
}

void DroneSimulator::detach(Observer* observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
        Logger::getInstance().log(Logger::DEBUG, "Observer detached from DroneSimulator");
    }
}

void DroneSimulator::notify() {
    for (Observer* observer : observers) {
        if (observer) {
            observer->update(droneData);
        }
    }
}

void DroneSimulator::startSimulation() {
    if (!isSimulationRunning) {
        isSimulationRunning = true;
        updateTimer->start();
        Logger::getInstance().log(Logger::INFO, "Drone simulation started");
        emit telemetryUpdated(droneData);
    }
}

void DroneSimulator::stopSimulation() {
    if (isSimulationRunning) {
        isSimulationRunning = false;
        updateTimer->stop();
        Logger::getInstance().log(Logger::INFO, "Drone simulation stopped");
    }
}

void DroneSimulator::setMovementStrategy(std::unique_ptr<MovementStrategy> strategy) {
    movementStrategy = std::move(strategy);
    if (movementStrategy) {
        Logger::getInstance().log(Logger::INFO, 
            QString("Movement strategy changed to: %1").arg(movementStrategy->getStrategyName()));
    }
}

void DroneSimulator::setFailureMode(bool enabled) {
    failureMode = enabled;
    Logger::getInstance().log(Logger::WARNING, 
        QString("Failure mode %1").arg(enabled ? "ENABLED" : "DISABLED"));

    if (enabled) {
        // Drop GPS fix
        droneData.setGPSStatus(GPSFixStatus::NO_FIX);
        // Increase battery drain rate significantly
        batteryDrainRate = 2.0;
    } else {
        // Restore GPS fix
        droneData.setGPSStatus(GPSFixStatus::FIX_3D);
        // Restore normal battery drain
        batteryDrainRate = 0.1;
    }
}

bool DroneSimulator::isRunning() const {
    return isSimulationRunning;
}

const DroneData& DroneSimulator::getDroneData() const {
    return droneData;
}

void DroneSimulator::updateTelemetry() {
    if (!isSimulationRunning) {
        return;
    }

    updateCount++;

    // Apply movement strategy if available
    applyMovementStrategy();

    // Update battery
    updateBattery();

    // Emit signal and notify observers
    emit telemetryUpdated(droneData);
    notify();

    // Log every 5th update to avoid spam
    if (updateCount % 5 == 0) {
        Logger::getInstance().log(Logger::INFO,
            QString("Telemetry updated - Lat: %1, Lon: %2, Battery: %3%")
            .arg(droneData.getLatitude(), 0, 'f', 6)
            .arg(droneData.getLongitude(), 0, 'f', 6)
            .arg(droneData.getBattery(), 0, 'f', 1));
    }
}

void DroneSimulator::initializeDrone() {
    droneData = DroneData("DRONE-001", 28.4595, 77.0266, 100.0, 0.0, 0.0, 100.0, GPSFixStatus::FIX_3D);
}

void DroneSimulator::updateBattery() {
    double currentBattery = droneData.getBattery();

    if (currentBattery > 0) {
        double newBattery = currentBattery - batteryDrainRate;
        newBattery = qMax(0.0, newBattery);
        droneData.setBattery(newBattery);

        // Log warning when battery gets low
        if (newBattery <= 20.0 && currentBattery > 20.0) {
            Logger::getInstance().log(Logger::WARNING, "Drone battery is low (20%)");
        }
        if (newBattery <= 5.0 && currentBattery > 5.0) {
            Logger::getInstance().log(Logger::ERROR, "Drone battery is critically low (5%)");
        }
    }
}

void DroneSimulator::applyMovementStrategy() {
    if (movementStrategy) {
        movementStrategy->updatePosition(droneData);
    }
}
