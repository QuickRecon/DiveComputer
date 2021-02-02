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
#include "LittleFS.h"

#include <cmath>

// Serial Port
#define EnableSerial // Disables screen PWM

// Button Parameters
#define BUTTON_POS_THRESHOLD 1000
#define BUTTON_NEG_THRESHOLD -300
#define BUTTON_1_CHANNEL 3
#define BUTTON_2_CHANNEL 2

// TFT Params
#define TFT_RST 0   // D3
#define TFT_RS  16   // D0
#define TFT_CS  2  // D8 SS
#ifndef EnableSerial
#define TFT_LED 1   // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!
#else
#define TFT_LED 0
#endif
#define TFT_BRIGHTNESS 255 // Initial brightness of TFT backlight (optional)

// RTC Params
#define RTC_CS 15
#define SET_CLOCK 0

struct RealTime;

struct UIData;

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
void PollButtons();

UIData CollectData();

#endif //DIVECOMPUTER_HWINTERFACE_H
