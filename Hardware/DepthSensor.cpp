//
// Created by aren on 2/8/21.
//

#include "DepthSensor.h"

DepthSensor::DepthSensor() {
    this->ready = sensor.init();
    this->surfacePressure = ReadPressure() / 1000.0;
}

double DepthSensor::ReadPressure() {
    sensor.read();
    sensor.pressure();
}

double DepthSensor::ReadTemperature() {
    sensor.read();
    sensor.temperature();
}
