//
// Created by aren on 27/10/21.
//

#include "HardwareManager.h"

HardwareManager::HardwareManager() {
    this->accelerometer = Accelerometer();
    this->adc = ADC();
    this->depthSensor = DepthSensor();
    this->io = IO();
    this->magnetometer = Magnetometer();
    this->rtc = RTC();
    this->tft = TFT();
}
