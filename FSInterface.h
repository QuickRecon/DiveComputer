//
// Created by aren on 4/2/21.
//

#ifndef DIVECOMPUTER_FSINTERFACE_H
#define DIVECOMPUTER_FSINTERFACE_H

#include "LittleFS.h"
#include "Deco.h"


#define DIVE_LOG_PATH "/Dives/"
#define SETTINGS_PATH "/settings.cfg"

#define MAX_GAS_COUNT 10

struct Settings {
    double GFHigh;
    double GFLow;
    double WaterDensity;
    Deco::Gas Gases[MAX_GAS_COUNT];
    double Tissues[16];
};

bool InitFS(FSInfo &fsInfo);

#endif //DIVECOMPUTER_FSINTERFACE_H
