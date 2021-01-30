//
// Created by aren on 30/1/21.
//
#include "UI.h"


bool SelfTest() {
    bool pass = true;

    int textRow = 15;

    Tft.clear();
    Tft.drawText(10, 10, "Starting Self Test:");

    textRow += 15;
    Tft.drawText(10, textRow, "RTC...");
    if (InitRTC()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Accelerometer...");
    if (!Accel.begin()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Accel.setRange(LSM303_RANGE_4G);
        Accel.setMode(LSM303_MODE_NORMAL);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Magnetometer...");
    if (!Mag.begin()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Mag.enableAutoRange(true);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "ADC1...");
    Adc1.begin();
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "ADC2...");
    Adc2.begin();
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "Depth Sensor...");
    if (!DepthSensor.init()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        DepthSensor.setModel(MS5837::MS5837_30BA);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Deco Model...");
    Deco *testDeco = new Deco;
    float depth = 60;
    int time = 60;
    //testDeco->AddGas(0,1,0);
    testDeco->AddDecent(MeterToBar(depth), MeterToBar(testDeco->DecentRate));
    testDeco->AddBottom(time);
    std::vector <Deco::DecoStop> Schedule = GetDecoSchedule(testDeco);
    if (testDeco->gases.size() == 0) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 25;
    Tft.setFont(Terminal12x16);
    if (pass) {
        Tft.drawText(10, textRow, "ALL TESTS PASSED", COLOR_GREEN);
    } else {
        Tft.drawText(10, textRow, "FAILED TESTS: HALT", COLOR_RED);
    }
    return pass;
}