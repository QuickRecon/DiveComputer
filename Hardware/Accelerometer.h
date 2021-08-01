//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_ACCELEROMETER_H
#define DIVECOMPUTER_ACCELEROMETER_H

#include <Adafruit_LSM303_Accel.h>

class Accelerometer {
public:
    Accelerometer();

private:
    Adafruit_LSM303_Accel_Unified sensor = Adafruit_LSM303_Accel_Unified(54321);
};


#endif //DIVECOMPUTER_ACCELEROMETER_H
