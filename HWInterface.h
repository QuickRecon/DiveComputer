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
#include <QR_ADS1115.h>
#include <Adafruit_Sensor.h>
#include "MS5837.h"
#include <Wire.h>
#include <SPI.h>
#include "FSInterface.h"
#include <ESP8266WiFi.h>
#include "PCF8574.h"

extern "C" {
#include "user_interface.h"
}

#include <cmath>

// Button Parameters
#define BUTTON_1_PIN P6 // On IO Expander
#define BUTTON_2_PIN P7 // On IO Expander

// TFT Params
#define TFT_RST 0   // NULL
#define TFT_RS  2   // D0
#define TFT_CS  15  // D8 SS
#define TFT_LED 16  // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!

// Reset circuit parameters
#define LATCH_ENABLE_RESET_ENABLE   0b11111111
#define LATCH_DISABLE_RESET_ENABLE  0b10011111
#define LATCH_ENABLE_RESET_DISABLE  0b01111111
#define LATCH_DISABLE_RESET_DISABLE 0b00011111

// ADC Params
#define ADC_1_V_PER_BIT 0.000188
#define ADC_ADDRESS 0x48
// RTC Params
#define RTC_CS 0
#define SET_CLOCK 0
// IO Expander Params
#define IO_READ_ADDRESS 0x20
#define IO_WRITE_ADDRESS 0x21
// Misc Params
#define BATTERY_PIN A0

struct RealTime;

struct UIData;

struct DepthSensorData {
    float Pressure;
    float Temperature;
};

struct CompassCalibrationCoefficients {
    float x;
    float y;
    float z;
};

// Accelerometer Params
extern Adafruit_LSM303_Accel_Unified Accel;

// Magnetometer Params
extern Adafruit_LSM303DLH_Mag_Unified Mag;
extern CompassCalibrationCoefficients CompassCalibration;

// ADC1 Params
extern QR_ADS1115 Adc;

// Depth Sensor Params
extern MS5837 DepthSensor;

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
extern TFT_22_ILI9225 Tft;

//extern PCF8574 IOExpander;

bool InitRTC();

bool InitDepth();

bool InitIO();

bool InitADC();

DepthSensorData ReadDepthSensor();

RealTime ReadRTC();

double ReadHeading();

void PollButtons();

UIData CollectData();

void TurnOff();

void EnableReset();

void DisableReset();

double ReadBatteryVoltage();

#endif //DIVECOMPUTER_HWINTERFACE_H
