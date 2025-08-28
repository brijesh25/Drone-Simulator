#ifndef DRONESIMULATOR_H
#define DRONESIMULATOR_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <vector>
#include "dronedata.h"
#include "observer.h"

class MovementStrategy;

class DroneSimulator : public QObject, public Subject {
    Q_OBJECT

public:
    explicit DroneSimulator(QObject *parent = nullptr);
    virtual ~DroneSimulator();

    // Observer pattern methods
    void attach(Observer* observer) override;
    void detach(Observer* observer) override;
    void notify() override;

    // Simulation methods
    void startSimulation();
    void stopSimulation();
    void setMovementStrategy(std::unique_ptr<MovementStrategy> strategy);
    void setFailureMode(bool enabled);
    bool isRunning() const;

    // Data access
    const DroneData& getDroneData() const;

public slots:
    void updateTelemetry();

signals:
    void telemetryUpdated(const DroneData& data);

private:
    void initializeDrone();
    void updateBattery();
    void applyMovementStrategy();

    DroneData droneData;
    QTimer* updateTimer;
    std::unique_ptr<MovementStrategy> movementStrategy;
    std::vector<Observer*> observers;

    bool isSimulationRunning;
    bool failureMode;
    int updateCount;
    double batteryDrainRate;
};

#endif // DRONESIMULATOR_H