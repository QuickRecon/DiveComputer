//
// Created by aren on 27/10/21.
//

#ifndef DIVECOMPUTER_HARDWAREMANAGER_H
#define DIVECOMPUTER_HARDWAREMANAGER_H

#include "Accelerometer.h"
#include "ADC.h"
#include "DepthSensor.h"
#include "IO.h"
#include "Magnetometer.h"
#include "RTC.h"
#include "TFT.h"

// Declare hardware parameters

// TFT Params
#define TFT_RST 0   // NULL
#define TFT_RS  2   // D0
#define TFT_CS  15  // D8 SS
#define TFT_LED 16  // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!

// Button Parameters


// IO Expander Params
#define IO_ADDRESS 0x20


class HardwareManager {
public:
    HardwareManager();

    Accelerometer accelerometer;
    ADC adc;
    DepthSensor depthSensor;
    IO io;
    Magnetometer magnetometer;
    RTC rtc;
    TFT tft;
};

HardwareManager hardwareManager;


#endif //DIVECOMPUTER_HARDWAREMANAGER_H
