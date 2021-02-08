//
// Created by aren on 4/2/21.
//
#include "FSInterface.h"

bool InitFS(FSInfo &fsInfo)
{
    bool status = LittleFS.begin();

    if (!LittleFS.exists(DIVE_LOG_PATH)) {
        LittleFS.mkdir(DIVE_LOG_PATH);
    }
    LittleFS.info(fsInfo);
    return status;
}

void WriteSettingsFile(Settings settings) {
    File settingsFile;
    LittleFS.open(SETTINGS_PATH, "w");

    char GFHigh[20];
    sprintf(GFHigh, "GFHigh:%f", settings.GFHigh);
    settingsFile.println(GFHigh);

    char GFLow[20];
    sprintf(GFLow, "GFLow:%f", settings.GFLow);
    settingsFile.println(GFLow);

    char waterDensity[25];
    sprintf(waterDensity, "WaterDensity:%f", settings.WaterDensity);
    settingsFile.println(waterDensity);

    for (int i = 0; i < MAX_GAS_COUNT; i++) {
        char gas[25];
        sprintf(gas, "Gas%d:%02.0f/%02.0f", i, settings.Gases[i].FrO2 * 100, settings.Gases[i].FrHe * 100);
        settingsFile.println(gas);
    }

    for (int i = 0; i < 16; i++) {
        char tissue[25];
        sprintf(tissue, "Tissue%d:%f", i, settings.Tissues[i]);
        settingsFile.println(tissue);
    }

    settingsFile.close();
}

Settings ReadSettingsFile() {
    Settings settings;
    File settingsFile;
    LittleFS.open(SETTINGS_PATH, "r");

    //settingsFile.readString();

    settingsFile.close();
}