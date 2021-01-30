//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_HWINTERFACE_H
#define DIVECOMPUTER_HWINTERFACE_H

#include "UI.h"
#include "DiveManager.h"

#include "TFT_22_ILI9225.h"
#include <SparkFunDS3234RTC.h>
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_Sensor.h>
#include "MS5837.h"
#include <Wire.h>
#include <SPI.h>

// TFT Params
#define TFT_RST 0   // D3
#define TFT_RS  16   // D0
#define TFT_CS  2  // D8 SS
#define TFT_LED 0   // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!

#define TFT_BRIGHTNESS 200 // Initial brightness of TFT backlight (optional)

struct RealTime;

struct DiveScreen;

// Accelerometer Params
extern Adafruit_LSM303_Accel_Unified Accel;

// Magnetometer Params
extern Adafruit_LSM303DLH_Mag_Unified Mag;

// ADC1 Params
extern Adafruit_ADS1115 Adc1;

// ADC2 Params
extern Adafruit_ADS1115 Adc2 ;

// Depth Sensor Params
extern MS5837 DepthSensor;

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
extern TFT_22_ILI9225 Tft;

struct DepthSensorData
{
    float Pressure;
    float Temperature;
};

bool InitRTC();
bool InitDepth();
DepthSensorData ReadDepthSensor();
RealTime ReadRTC();
double ReadHeading();


DiveScreen CollectData();

#endif //DIVECOMPUTER_HWINTERFACE_H
