#ifndef OBSERVER_H
#define OBSERVER_H

class DroneData;

// Observer Pattern Implementation
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const DroneData& data) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
};

#endif // OBSERVER_H