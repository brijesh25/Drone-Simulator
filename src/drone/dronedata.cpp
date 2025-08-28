#include "dronedata.h"

DroneData::DroneData()
    : droneId("DRONE-001")
    , latitude(28.4595)
    , longitude(77.0266)
    , altitude(100.0)
    , heading(0.0)
    , speed(0.0)
    , battery(100.0)
    , gpsStatus(GPSFixStatus::FIX_3D)
{
}

DroneData::DroneData(const QString& id, double lat, double lon, double alt, 
                     double heading, double speed, double battery, GPSFixStatus gps)
    : droneId(id)
    , latitude(lat)
    , longitude(lon)
    , altitude(alt)
    , heading(heading)
    , speed(speed)
    , battery(battery)
    , gpsStatus(gps)
{
}

QString DroneData::gpsStatusString() const {
    switch (gpsStatus) {
        case GPSFixStatus::NO_FIX: return "No Fix";
        case GPSFixStatus::FIX_2D: return "2D Fix";
        case GPSFixStatus::FIX_3D: return "3D Fix";
        default: return "Unknown";
    }
}
