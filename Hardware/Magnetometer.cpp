//
// Created by aren on 2/8/21.
//

#include "Magnetometer.h"

Magnetometer::Magnetometer() {
    // No initialization needed
}

void Magnetometer::Calibrate() {

}

double Magnetometer::ReadHeading() {
    sensors_event_t event;
    sensor.getEvent(&event);

    double x = event.magnetic.x - CompassCalibration.x;
    double y = event.magnetic.y - CompassCalibration.y;
    double z = event.magnetic.z - CompassCalibration.z;


    double Pi = 3.14159;

    // Calculate the angle of the vector y,x
    double heading = (atan2(x, y) * 180) / Pi;

    // Normalize to 0-360
    if (heading < 0) {
        heading = 360 + heading;
    }

    return heading;
}
