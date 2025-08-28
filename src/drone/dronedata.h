#ifndef DRONEDATA_H
#define DRONEDATA_H

#include <QString>

enum class GPSFixStatus {
    NO_FIX = 0,
    FIX_2D = 1,
    FIX_3D = 2
};

class DroneData {
public:
    DroneData();
    DroneData(const QString& id, double lat, double lon, double alt, 
              double heading, double speed, double battery, GPSFixStatus gps);

    // Getters
    QString getId() const { return droneId; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    double getAltitude() const { return altitude; }
    double getHeading() const { return heading; }
    double getSpeed() const { return speed; }
    double getBattery() const { return battery; }
    GPSFixStatus getGPSStatus() const { return gpsStatus; }

    // Setters
    void setId(const QString& id) { droneId = id; }
    void setLatitude(double lat) { latitude = lat; }
    void setLongitude(double lon) { longitude = lon; }
    void setAltitude(double alt) { altitude = alt; }
    void setHeading(double h) { heading = h; }
    void setSpeed(double s) { speed = s; }
    void setBattery(double b) { battery = b; }
    void setGPSStatus(GPSFixStatus status) { gpsStatus = status; }

    // Utility
    QString gpsStatusString() const;

private:
    QString droneId;
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double speed;
    double battery;
    GPSFixStatus gpsStatus;
};

#endif // DRONEDATA_H