//
// Created by aren on 4/2/21.
//

#ifndef DIVECOMPUTER_FSINTERFACE_H
#define DIVECOMPUTER_FSINTERFACE_H

#include "LittleFS.h"
#include "Deco.h"
#include "UI.h"
#include "DiveManager.h"
#include "HWInterface.h"

#define DIVE_LOG_PATH "/Dives/"
#define SETTINGS_PATH "/settings.cfg"

#define MAX_GAS_COUNT 10

//#define FORCE_RESET

struct UIData;
struct CompassCalibrationCoefficients;
#ifndef REALTIME
#define REALTIME
struct RealTime {
    int Second;
    int Minute;
    int Hour;

    int Day;
    int Month;
    int Year;
};
#else
struct RealTime;
#endif

struct Settings {
    double GFHigh;
    double GFLow;
    double WaterDensity;
    int GasCount;
    Deco::Gas Gases[MAX_GAS_COUNT];
    double TissueN2[16];
    double TissueHe[16];
    double CNS;
    double OTU;
    float CompassCalX;
    float CompassCalY;
    float CompassCalZ;
    double LastDepth;
    double LastTime;
    RealTime Time;
};

extern File CurrDiveLog;
extern char SampleStartText[20];

bool InitFS(FSInfo &fsInfo);

void WriteSettingsFile(Settings settings);

Settings GenerateSettings();

void ApplySettings(Settings settings);

Settings ReadSettingsFile();

void OpenDiveLog();

void CloseDiveLog();

void EnterInDiveLog(UIData data);

std::vector<std::string> ListDives();

#endif //DIVECOMPUTER_FSINTERFACE_H
