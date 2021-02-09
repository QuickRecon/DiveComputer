//
// Created by aren on 4/2/21.
//

#ifndef DIVECOMPUTER_FSINTERFACE_H
#define DIVECOMPUTER_FSINTERFACE_H

#include "LittleFS.h"
#include "Deco.h"
#include "UI.h"
#include "DiveManager.h"


#define DIVE_LOG_PATH "/Dives/"
#define SETTINGS_PATH "/settings.cfg"

#define MAX_GAS_COUNT 10

//#define FORCE_RESET

struct UIData;

struct Settings {
    double GFHigh;
    double GFLow;
    double WaterDensity;
    int GasCount;
    Deco::Gas Gases[MAX_GAS_COUNT];
    double TissueN2[16];
    double TissueHe[16];
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

#endif //DIVECOMPUTER_FSINTERFACE_H
