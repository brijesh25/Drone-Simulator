#include "drone.h"
#include "logger.h"

Drone::Drone(QObject *parent)
    : QObject(parent)
{
    Logger::getInstance().log(Logger::INFO, "Drone object created");
}

void Drone::update(const DroneData& data) {
    currentData = data;
    emit dataChanged(data);

    Logger::getInstance().log(Logger::DEBUG, 
        QString("Drone data updated - ID: %1, Battery: %2%")
        .arg(data.getId())
        .arg(data.getBattery(), 0, 'f', 1));
}

const DroneData& Drone::getData() const {
    return currentData;
}