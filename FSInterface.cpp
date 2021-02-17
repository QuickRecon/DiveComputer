//
// Created by aren on 4/2/21.
//
#include "FSInterface.h"

File CurrDiveLog;
char SampleStartText[20];

bool InitFS(FSInfo &fsInfo) {
    bool status = LittleFS.begin();

#ifdef FORCE_RESET
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
    char stringBuffer[100];

    sprintf(stringBuffer, "GFHigh:%16.15f", settings.GFHigh);
    settingsFile.println(stringBuffer);

    sprintf(stringBuffer, "GFLow:%16.15f", settings.GFLow);
    settingsFile.println(stringBuffer);

    sprintf(stringBuffer, "WaterDensity:%16.15f", settings.WaterDensity);
    settingsFile.println(stringBuffer);

    for (int i = 0; i < settings.GasCount; i++) {
        sprintf(stringBuffer, "Gas:%d:%02.0f/%02.0f", i, settings.Gases[i].FrO2 * 100, settings.Gases[i].FrHe * 100);
        settingsFile.println(stringBuffer);
    }

    for (int i = 0; i < 16; i++) {
        sprintf(stringBuffer, "TissueN2:%d:%16.15f", i, settings.TissueN2[i]);
        settingsFile.println(stringBuffer);
    }

    for (int i = 0; i < 16; i++) {
        sprintf(stringBuffer, "TissueHe:%d:%16.15f", i, settings.TissueHe[i]);
        settingsFile.println(stringBuffer);
    }

    sprintf(stringBuffer, "Time:%04d-%02d-%02d %02d-%02d-%02d", settings.Time.Year, settings.Time.Month,
            settings.Time.Day, settings.Time.Hour, settings.Time.Minute,
            settings.Time.Second);
    settingsFile.println(stringBuffer);


    sprintf(stringBuffer, "LastDive:%16.15f:%16.15f", settings.LastTime, settings.LastDepth);
    settingsFile.println(stringBuffer);

    sprintf(stringBuffer, "CNS:%16.15f", settings.CNS);
    settingsFile.println(stringBuffer);

    sprintf(stringBuffer, "OTU:%16.15f", settings.CNS);
    settingsFile.println(stringBuffer);

    sprintf(stringBuffer, "Compass:%16.15f:%16.15f:%16.15f", settings.CompassCalX, settings.CompassCalY,
            settings.CompassCalZ);
    settingsFile.println(stringBuffer);

    settingsFile.close();
}

Settings GenerateSettings() {
    Settings settings;
    settings.GFHigh = DecoActual.GFHigh;
    settings.GFLow = DecoActual.GFLow;
    settings.WaterDensity = WaterDensity;
    settings.CNS = CNS;
    settings.OTU = OTUs;

    settings.CompassCalX = CompassCalibration.x;
    settings.CompassCalY = CompassCalibration.y;
    settings.CompassCalZ = CompassCalibration.z;

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

    settings.Time = ReadRTC();

    settings.LastDepth = LastDiveDepth;
    settings.LastTime = LastDiveTime;

    return settings;
}

void ApplySettings(Settings settings) {
    DecoActual.GFHigh = settings.GFHigh;
    DecoActual.GFLow = settings.GFLow;
    WaterDensity = settings.WaterDensity;
    LastDiveDepth = settings.LastDepth;
    LastDiveTime = settings.LastTime;
    CNS = settings.CNS;
    OTUs = settings.OTU;

    CompassCalibration.x = settings.CompassCalX;
    CompassCalibration.y = settings.CompassCalY;
    CompassCalibration.z = settings.CompassCalZ;

    DecoActual.Gases.clear();
    DecoActual.AddDecent(1, ASCENT_RATE); // Set the depth to 1
    for (int i = 0; i < settings.GasCount; i++) {
        DecoActual.AddGas(settings.Gases[i].FrN2, settings.Gases[i].FrO2, settings.Gases->FrHe);
    }

    for (int i = 0; i < 16; i++) {
        DecoActual.Pn[i] = settings.TissueN2[i];
    }

    for (int i = 0; i < 16; i++) {
        DecoActual.Ph[i] = settings.TissueHe[i];
    }

    // Propagate the tissues forward in Time
    double diff = TimeDiff(ReadRTC(), settings.Time);
    DecoActual.AddDecent(1, DESCENT_RATE);
    DecoActual.AddBottom(diff);
    DecayO2Exposure(diff);
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
            if (strcmp(propertyName, "GFHi") == 0) {
                strtok(nullptr,
                       &settingDelimiter); // Flush the next item in the buffer, for some reason g is triggering strtok
                char *GFHigh = strtok(nullptr, &settingDelimiter);
                settings.GFHigh = strtod(GFHigh, &GFHigh);
            } else if (strcmp(propertyName, "GFLow") == 0) {
                char *GFLow = strtok(nullptr, &settingDelimiter);
                settings.GFLow = strtod(GFLow, &GFLow);
            } else if (strcmp(propertyName, "CNS") == 0) {
                char *cns = strtok(nullptr, &settingDelimiter);
                settings.CNS = strtod(cns, &cns);
            } else if (strcmp(propertyName, "OTU") == 0) {
                char *otu = strtok(nullptr, &settingDelimiter);
                settings.OTU = strtod(otu, &otu);
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
            } else if (strcmp(propertyName, "Time") == 0) {
                char *time = strtok(nullptr, &settingDelimiter);
                sscanf(time, "%d-%d-%d %d-%d-%d", &settings.Time.Year, &settings.Time.Month,
                       &settings.Time.Day, &settings.Time.Hour, &settings.Time.Minute, &settings.Time.Second);
            } else if (strcmp(propertyName, "LastDive") == 0) {
                char *LastTime = strtok(nullptr, &settingDelimiter);
                settings.LastTime = strtod(LastTime, &LastTime);
                char *lastDepth = strtok(nullptr, &settingDelimiter);
                settings.LastTime = strtod(lastDepth, &lastDepth);
            } else if (strcmp(propertyName, "Compass") == 0) {
                char *x = strtok(nullptr, &settingDelimiter);
                char *y = strtok(nullptr, &settingDelimiter);
                char *z = strtok(nullptr, &settingDelimiter);

                settings.CompassCalX = (float) strtod(x, &x);
                settings.CompassCalY = (float) strtod(y, &y);
                settings.CompassCalZ = (float) strtod(z, &z);
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
    CurrDiveLog.println(R"lit("date","Time","sample Time (min)","sample depth (m)","sample temperature (C)")lit");
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

std::vector<std::string> ListDives() {
    Dir diveDir = LittleFS.openDir(DIVE_LOG_PATH);
    std::vector<std::string> diveVector;
    while (diveDir.next()) {
        Serial.print(diveDir.fileName());
        diveVector.emplace_back(diveDir.fileName().c_str());
    }
    return diveVector;
}