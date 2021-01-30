//
// Created by aren on 30/1/21.
//
#include "HWInterface.h"

#include <cmath>

// Accelerometer Params
Adafruit_LSM303_Accel_Unified Accel = Adafruit_LSM303_Accel_Unified(54321);

// Magnetometer Params
Adafruit_LSM303DLH_Mag_Unified Mag = Adafruit_LSM303DLH_Mag_Unified(12345);

// ADC1 Params
Adafruit_ADS1115 Adc1 =  Adafruit_ADS1115(0x49);

// ADC2 Params
Adafruit_ADS1115 Adc2 =  Adafruit_ADS1115(0x48);

// Depth Sensor Params
MS5837 DepthSensor;

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 Tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);


bool InitRTC() {
    rtc.autoTime();
    rtc.update();
    bool status = rtc.second() <= 60;
    return status; // Sanity check to ensure RTC is working
}

DepthSensorData ReadDepthSensor()
{
    DepthSensor.read();
    DepthSensorData data{};
    data.Pressure = DepthSensor.pressure();
    data.Temperature = DepthSensor.temperature();
    return data;
}

RealTime ReadRTC()
{
    rtc.update();
    RealTime data{};

    data.Day = rtc.date();
    data.Month = rtc.month();
    data.Year = rtc.year();

    data.Hour = rtc.hour();
    data.Minute = rtc.minute();
    data.Second = rtc.second();

    return data;
}

double GetHeading()
{
    sensors_event_t event;
    Mag.getEvent(&event);

    double Pi = 3.14159;

    // Calculate the angle of the vector y,x
    double heading = (std::atan2(event.magnetic.z,event.magnetic.x) * 180) / Pi;

    // Normalize to 0-360
    if (heading < 0)
    {
        heading = 360 + heading;
    }

    return heading;
}

DiveScreen CollectData()
{
    DiveScreen screenData{};

    DepthSensorData depthData = ReadDepthSensor();
    screenData.AmbientPressure = depthData.Pressure/1000.0;
    screenData.Depth = BarToMeter(depthData.Pressure/1000.0);
    screenData.Temperature = depthData.Temperature;

    screenData.Heading = GetHeading();

    return screenData;
}