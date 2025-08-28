#ifndef DRONE_H
#define DRONE_H

#include <QObject>
#include "dronedata.h"
#include "observer.h"

class Drone : public QObject, public Observer {
    Q_OBJECT

public:
    explicit Drone(QObject *parent = nullptr);

    // Observer pattern implementation
    void update(const DroneData& data) override;

    const DroneData& getData() const;

signals:
    void dataChanged(const DroneData& data);

private:
    DroneData currentData;
};

#endif // DRONE_H