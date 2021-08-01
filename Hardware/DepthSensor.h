//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_DEPTHSENSOR_H
#define DIVECOMPUTER_DEPTHSENSOR_H

#include "MS5837.h"

class DepthSensor {
public:
    DepthSensor();

    double ReadPressure();

    double ReadTemperature();

    bool check();

private:
    MS5837 sensor;
    double surfacePressure;
    bool ready;
};


#endif //DIVECOMPUTER_DEPTHSENSOR_H
