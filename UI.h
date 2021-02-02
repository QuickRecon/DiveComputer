//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_UI_H
#define DIVECOMPUTER_UI_H

#include "HWInterface.h"
#include "DiveManager.h"

#define ROW_1 10
#define ROW_2 50
#define ROW_3 90
#define ROW_4 130

#define VAL_OFFSET 15

#define COLUMN_1 10
#define COLUMN_2 60
#define COLUMN_3 110
#define COLUMN_4 160

extern bool ShowingDiveScreen;
extern bool ShowingSurfaceScreen;

struct RealTime {
    int Second;
    int Minute;
    int Hour;

    int Day;
    int Month;
    int Year;
};

struct UIData {
    double Depth;
    double AmbientPressure;
    double Temperature;
    double DiveTime;
    Deco::DecoStop Stop;
    double NDL;
    Deco::Gas Gas;
    double PPO2;
    double Heading;
    double Rate;
    RealTime Time;
    double AverageDepth;
    double TTS;
    double CNS;
};

struct Bar {
    int OriginX;
    int OriginY;
    int Length;
    int Thickness;
    bool Vertical;
    int Color;
    double MaxValue;
    bool EndStop;
};

bool SelfTest();

void ShowDiveScreen(UIData data);

void ShowSurfaceScreen(UIData data);

void ButtonOne();

void ButtonTwo();


#endif //DIVECOMPUTER_UI_H
