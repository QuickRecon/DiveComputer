//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_MAGNETOMETER_H
#define DIVECOMPUTER_MAGNETOMETER_H

#include <Adafruit_LSM303DLH_Mag.h>
#include "Accelerometer.h"

struct CompassCalibrationCoefficients {
    float x;
    float y;
    float z;
};

class Magnetometer {
public:
    Magnetometer();

    void Calibrate();

    double ReadHeading();

private:
    Adafruit_LSM303DLH_Mag_Unified sensor = Adafruit_LSM303DLH_Mag_Unified(12345);
    CompassCalibrationCoefficients CompassCalibration{0, 0, 0};
    Accelerometer accl = Accelerometer();
};


#endif //DIVECOMPUTER_MAGNETOMETER_H
