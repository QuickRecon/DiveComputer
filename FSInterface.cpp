//
// Created by aren on 4/2/21.
//
#include "FSInterface.h"

File CurrDiveLog;
char SampleStartText[20];

bool InitFS(FSInfo &fsInfo) {
    bool status = LittleFS.begin();

#ifdef FORCE_RESET
    LittleFS.mkdir(DIVE_LOG_PATH);

    Settings settings = GenerateSettings();
    WriteSettingsFile(settings);
    settings = ReadSettingsFile();
    ApplySettings(settings);
#else
    if (!LittleFS.exists(DIVE_LOG_PATH)) {
        LittleFS.mkdir(DIVE_LOG_PATH);
    }

    if (!LittleFS.exists(SETTINGS_PATH)) { // Init with the defaults if the settings file is not found
        Settings settings = GenerateSettings();
        WriteSettingsFile(settings);
    } else { // Load up the settings
        Settings settings = ReadSettingsFile();
        ApplySettings(settings);
    }
#endif

    LittleFS.info(fsInfo);
    return status;
}

void WriteSettingsFile(Settings settings) {
    File settingsFile = LittleFS.open(SETTINGS_PATH, "w");

    char GFHigh[255];
    sprintf(GFHigh, "GFHigh:%f", settings.GFHigh);
    settingsFile.println(GFHigh);

    char GFLow[255];
    sprintf(GFLow, "GFLow:%f", settings.GFLow);
    settingsFile.println(GFLow);

    char waterDensity[255];
    sprintf(waterDensity, "WaterDensity:%f", settings.WaterDensity);
    settingsFile.println(waterDensity);

    for (int i = 0; i < settings.GasCount; i++) {
        char gas[255];
        sprintf(gas, "Gas:%d:%02.0f/%02.0f", i, settings.Gases[i].FrO2 * 100, settings.Gases[i].FrHe * 100);
        settingsFile.println(gas);
    }

    for (int i = 0; i < 16; i++) {
        char tissue[255];
        sprintf(tissue, "TissueN2:%d:%f", i, settings.TissueN2[i]);
        settingsFile.println(tissue);
    }

    for (int i = 0; i < 16; i++) {
        char tissue[255];
        sprintf(tissue, "TissueHe:%d:%f", i, settings.TissueHe[i]);
        settingsFile.println(tissue);
    }
    settingsFile.close();
}

Settings GenerateSettings() {
    Settings settings;
    settings.GFHigh = DecoActual.GFHigh;
    settings.GFLow = DecoActual.GFLow;
    settings.WaterDensity = WaterDensity;

    for (int i = 0; i < DecoActual.Gases.size() && i < MAX_GAS_COUNT; i++) {
        settings.GasCount = i + 1;
        settings.Gases[i].FrHe = DecoActual.Gases[i].FrHe;
        settings.Gases[i].FrO2 = DecoActual.Gases[i].FrO2;
        settings.Gases[i].FrN2 = DecoActual.Gases[i].FrN2;
    }

    for (int i = 0; i < 16; i++) {
        settings.TissueN2[i] = DecoActual.Pn[i];
    }

    for (int i = 0; i < 16; i++) {
        settings.TissueHe[i] = DecoActual.Ph[i];
    }

    return settings;
}

void ApplySettings(Settings settings) {
    DecoActual.GFHigh = settings.GFHigh;
    DecoActual.GFLow = settings.GFLow;
    WaterDensity = settings.WaterDensity;

    DecoActual.Gases.clear();
    for (int i = 0; i < settings.GasCount; i++) {
        DecoActual.AddGas(settings.Gases[i].FrN2, settings.Gases[i].FrO2, settings.Gases->FrHe);
    }

    for (int i = 0; i < 16; i++) {
        DecoActual.Pn[i] = settings.TissueN2[i];
    }

    for (int i = 0; i < 16; i++) {
        DecoActual.Ph[i] = settings.TissueHe[i];
    }
}

Settings ReadSettingsFile() {
    Settings settings;
    File settingsFile = LittleFS.open(SETTINGS_PATH, "r");
    char settingDelimiter = ':';
    char gasDelimiter = '/';

    settings.GasCount = 0;
    char line[255];
    int i = 0;
    while (settingsFile.available()) {
        char currChar = (char) settingsFile.read();
        if (currChar != '\n') {
            line[i] = currChar;
            line[i + 1] = '\0';
            i++;
        } else {
            // Process line
            char *propertyName = strtok(line, &settingDelimiter);
            if (strcmp(propertyName, "GFHigh") == 0) {
                char *GFHigh = strtok(nullptr, &settingDelimiter);
                settings.GFHigh = strtod(GFHigh, &GFHigh);
            } else if (strcmp(propertyName, "GFLow") == 0) {
                char *GFLow = strtok(nullptr, &settingDelimiter);
                settings.GFLow = strtod(GFLow, &GFLow);
            } else if (strcmp(propertyName, "WaterDensity") == 0) {
                char *waterDensity = strtok(nullptr, &settingDelimiter);
                settings.WaterDensity = strtod(waterDensity, &waterDensity);
            } else if (strcmp(propertyName, "Gas") == 0) {
                char *GasIndexString = strtok(nullptr, &settingDelimiter);
                char *GasString = strtok(nullptr, &settingDelimiter);
                char *frO2String = strtok(GasString, &gasDelimiter);
                char *frHeString = strtok(nullptr, &gasDelimiter);
                int GasIndex = atoi(GasIndexString);
                double frO2 = strtod(frO2String, &frO2String) / 100.0;
                double frHe = strtod(frHeString, &frHeString) / 100.0;

                settings.Gases[GasIndex].FrO2 = frO2;
                settings.Gases[GasIndex].FrHe = frHe;
                settings.Gases[GasIndex].FrN2 = 1 - frO2 - frHe;
                settings.GasCount++;

            } else if (strcmp(propertyName, "TissueN2") == 0) {
                char *TissueIndexString = strtok(nullptr, &settingDelimiter);
                char *TissueString = strtok(nullptr, &settingDelimiter);

                int TissueIndex = atoi(TissueIndexString);
                double tissue = strtod(TissueString, &TissueString);

                settings.TissueN2[TissueIndex] = tissue;
            } else if (strcmp(propertyName, "TissueHe") == 0) {
                char *TissueIndexString = strtok(nullptr, &settingDelimiter);
                char *TissueString = strtok(nullptr, &settingDelimiter);

                int TissueIndex = atoi(TissueIndexString);
                double tissue = strtod(TissueString, &TissueString);

                settings.TissueHe[TissueIndex] = tissue;
            }

            // nuke line
            line[0] = '\0';
            i = 0;
        }
    }
    return settings;
}

void OpenDiveLog() {
    char dateTime[24];
    RealTime time = ReadRTC();
    sprintf(dateTime, "%04d-%02d-%02d %02d:%02d:%02d.csv", time.Year, time.Month, time.Day, time.Hour, time.Minute,
            time.Second);

    char filename[32];
    strcpy(filename, DIVE_LOG_PATH);
    strcat(filename, dateTime);

    CurrDiveLog = LittleFS.open(filename, "w");
    CurrDiveLog.println(R"lit("date","time","sample time (min)","sample depth (m)","sample temperature (C)")lit");
    sprintf(SampleStartText, "%04d-%02d-%02d,%02d:%02d:%02d", time.Year, time.Month, time.Day, time.Hour, time.Minute,
            time.Second);
}

void CloseDiveLog() {
    CurrDiveLog.close();
}

void EnterInDiveLog(UIData data) {
    int minutes = floor(data.DiveTime);
    int seconds = floor((data.DiveTime - minutes) * 60.0);
    char row[255];
    sprintf(row, "%s,%02d:%02d,%.1f,%f.1", SampleStartText, minutes, seconds, data.Depth, data.Temperature);
    CurrDiveLog.println(row);
}