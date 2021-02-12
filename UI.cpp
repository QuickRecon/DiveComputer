//
// Created by aren on 30/1/21.
//
#include "UI.h"

// UI Elements
Bar DiveTimeBar = {.OriginX=COLUMN_2, .OriginY=ROW_1 +
                                               10, .Length=28, .Thickness=2, .Vertical=false, .Color=COLOR_WHITE, .MaxValue=1, .EndStop=true};
Bar TimeBar = {.OriginX=COLUMN_4 + 18, .OriginY=ROW_4 + VAL_OFFSET +
                                                15, .Length=15, .Thickness=2, .Vertical=true, .Color=COLOR_WHITE, .MaxValue=60, .EndStop=true};

// Menu Items
MenuItem MenuSwitchGas = MenuItem("Switch Gas", &SwitchGasCallback, true);
MenuItem MenuEndDive = MenuItem("End Dive", &EndDiveCallback, true);
MenuItem MenuTurnOff = MenuItem("Turn Off", &TurnOffCallback, true);
MenuItem MenuStartWeb = MenuItem("Start Web", &StartWebCallback, true);
MenuItem MenuStartDive = MenuItem("Start Dive", &StartDiveCallback, true);
MenuItem CalibrateCompass = MenuItem("Calibrate Compass", &CalibrateCompassCallback, true);

// Menu Structure
MenuItem SurfaceItems[] = {MenuSwitchGas, MenuStartDive, MenuStartWeb, CalibrateCompass, MenuTurnOff};
MenuItem DiveItems[] = {MenuSwitchGas, MenuEndDive};
MenuItem GasItems[MAX_GAS_COUNT] = {};

Menu SurfaceMenu = Menu(SurfaceItems, 5, 0);
Menu DiveMenu = Menu(DiveItems, 2, 0);
Menu GasMenu = Menu(GasItems,MAX_GAS_COUNT, 0);

// Stage
UIState CurrUIState = {.Mode=SURFACE,.InMenu=false,.Menu=&SurfaceMenu,.ClearNeeded=true};

void UpdateUI(UIData data)
{
    if(CurrUIState.ClearNeeded)
    {
        Tft.clear();
        CurrUIState.ClearNeeded = false;
    }
    if (CurrUIState.Mode == DIVE) {
        if(CurrUIState.InMenu){
            ShowDiveTopRow(data);
            ShowBottomRow(data);
            ShowMenu(*CurrUIState.Menu);
        }
        else {
            ShowDiveScreen(data);
        }
    } else if (CurrUIState.Mode == SURFACE) {
        if (CurrUIState.InMenu) {
            ShowBottomRow(data);
            ShowMenu(*CurrUIState.Menu);
        } else {
            ShowSurfaceScreen(data);
        }
    }

    // Lock out end dive when below 1m
    if (data.Depth > 1.0 && MenuEndDive.Enabled) {
        MenuEndDive.Enabled = false;
    } else if (!MenuEndDive.Enabled) {
        MenuEndDive.Enabled = true;
    }
}

void ShowMenu(Menu menu) {
    Tft.setFont(Terminal12x16);
    Tft.drawText(COLUMN_1, ROW_2, menu.Items[menu.CurrIndex].MenuText);
}

void ShowBar(Bar bar, double value) {
    int EndX, EndY, ValX, ValY;
    if (bar.Vertical) {
        ValX = bar.OriginX + bar.Thickness;
        ValY = bar.OriginY - (int) ceil(value / bar.MaxValue * bar.Length);

        EndX = bar.OriginX + bar.Thickness;
        EndY = bar.OriginY - bar.Length;
    } else {
        ValY = bar.OriginY + bar.Thickness;
        ValX = bar.OriginX + (int) ceil(value / bar.MaxValue * bar.Length);

        EndY = bar.OriginY + bar.Thickness;
        EndX = bar.OriginX + bar.Length;
    }
    Tft.fillRectangle(bar.OriginX, bar.OriginY, ValX, ValY, bar.Color);
    if ((int) ceil(value / bar.MaxValue * bar.Length) != bar.Length) {
        Tft.fillRectangle(ValX - bar.Thickness, ValY - bar.Thickness, EndX, EndY,
                          COLOR_BLACK); // Blank Out any part of the bar that isn't displayed
    }
    if (bar.EndStop) {
        if (bar.Vertical) {
            Tft.fillRectangle(EndX - bar.Thickness, EndY - 1, EndX, EndY - 1, bar.Color);
        } else {
            Tft.fillRectangle(EndX - 1, EndY - bar.Thickness, EndX - 1, EndY, bar.Color);
        }
    }
}

void ShowDiveTopRow(UIData data) {
    Tft.setFont(Terminal6x8);
    Tft.drawText(COLUMN_1, ROW_1, "Depth");
    Tft.drawText(COLUMN_2, ROW_1, "Time");
    if (data.NDL == -1) {
        Tft.drawText(COLUMN_3, ROW_1, "Stop");
        Tft.drawText(COLUMN_4, ROW_1, "S Time");
    } else {
        Tft.drawText((COLUMN_3 + COLUMN_4) / 2, ROW_1, "NDL");
    }

    Tft.setFont(Terminal11x16);
    // Depth
    char depth[7];
    sprintf(depth, "%04.1f", fabs(data.Depth));
    Tft.drawText(COLUMN_1, ROW_1 + VAL_OFFSET, depth);

    // Dive Time
    char diveTime[7];
    sprintf(diveTime, "%3.0f", floor(fabs(data.DiveTime)));
    Tft.drawText(COLUMN_2, ROW_1 + VAL_OFFSET, diveTime);
    ShowBar(DiveTimeBar, data.DiveTime - (double) ((int) data.DiveTime));

    if (data.NDL == -1) {
        char stop[4];
        sprintf(stop, "%.0f", data.Stop.Depth);
        char stime[4];
        sprintf(stime, "%.0f", data.Stop.Time);
        Tft.drawText(COLUMN_3, ROW_1 + VAL_OFFSET, stop);
        Tft.drawText(COLUMN_4, ROW_1 + VAL_OFFSET, stime);
    } else {
        char ndl[4];
        sprintf(ndl, "%.0f", data.NDL);
        Tft.drawText((COLUMN_3 + COLUMN_4) / 2, ROW_1 + VAL_OFFSET, ndl);
    }

    Tft.drawRectangle(0, ROW_1 + VAL_OFFSET + 18, Tft.maxX(), ROW_1 + VAL_OFFSET + 18, COLOR_WHITE);
}

void ShowBottomRow(UIData data) {
    Tft.setFont(Terminal6x8);

    Tft.drawText((COLUMN_1 + COLUMN_2) / 2, ROW_4, "Gas");
    Tft.drawText((COLUMN_3 + COLUMN_4) / 2, ROW_4, "Clock");

    Tft.setFont(Terminal11x16);

    // Gas
    char gas[7];
    sprintf(gas, "%02.0f/%02.0f", data.Gas.FrO2 * 100, data.Gas.FrHe * 100);
    Tft.drawText(COLUMN_1 + 10, ROW_4 + VAL_OFFSET, gas);

    // Real Time
    char time[7];
    sprintf(time, "%02d:%02d", data.Time.Hour, data.Time.Minute);
    Tft.drawText(COLUMN_3 + 10, ROW_4 + VAL_OFFSET, time);
    ShowBar(TimeBar, data.Time.Second);

    Tft.drawRectangle(0, ROW_4 - 3, Tft.maxX(), ROW_4 - 3, COLOR_WHITE);
}

void ShowSurfaceScreen(UIData data) {
    ShowBottomRow(data);
    Tft.setFont(Terminal6x8);
    Tft.drawText(COLUMN_1, ROW_1, "Surface Pressure");
    Tft.drawText(COLUMN_3 + 20, ROW_1, "Temperature");

    Tft.drawText((COLUMN_1 + COLUMN_2) / 2 - 10, ROW_2, "GFL/GFH");
    Tft.drawText((COLUMN_1 + COLUMN_2) / 2, ROW_3, "CNS");

    Tft.drawText(COLUMN_3, ROW_2, "L Dpth");
    Tft.drawText(COLUMN_4, ROW_2, "Bat V.");
    Tft.drawText(COLUMN_3, ROW_3, "L Time");
    Tft.drawText(COLUMN_4, ROW_3, "Head.");

    Tft.setFont(Terminal11x16);
    char pressure[10];
    sprintf(pressure, "%.3fBar", fabs(data.AmbientPressure));
    Tft.drawText(COLUMN_1, ROW_1 + VAL_OFFSET, pressure);

    char temperature[10];
    sprintf(temperature, "%02.1fC", data.Temperature);
    Tft.drawText(COLUMN_3 + 25, ROW_1 + VAL_OFFSET, temperature);

    char gradientFactors[10];
    sprintf(gradientFactors, "%02.0f/%02.0f", DecoActual.GFLow * 100, DecoActual.GFHigh * 100);
    Tft.drawText(COLUMN_1 + 10, ROW_2 + VAL_OFFSET, gradientFactors);

    char cns[7];
    sprintf(cns, "%02.0f", data.CNS);
    Tft.drawText((COLUMN_1 + COLUMN_2) / 2, ROW_3 + VAL_OFFSET, cns);

    char battery[7];
    sprintf(battery, "%.2fV", ReadBatteryVoltage());
    Tft.drawText(COLUMN_4, ROW_2 + VAL_OFFSET, battery);

    char lastDiveDepth[7];
    sprintf(lastDiveDepth, "%04.1f", LastDiveDepth);
    Tft.drawText(COLUMN_3, ROW_2 + VAL_OFFSET, lastDiveDepth);

    char LastTime[7];
    sprintf(LastTime, "%.0f", floor(LastDiveTime));
    Tft.drawText(COLUMN_3, ROW_3 + VAL_OFFSET, LastTime);

    // heading
    char heading[7];
    sprintf(heading, "%03.0f", data.Heading);
    Tft.drawText(COLUMN_4, ROW_3 + VAL_OFFSET, heading);
}

void ShowDiveScreen(UIData data) {
    CurrUIState.ClearNeeded = false;

    ShowDiveTopRow(data);
    ShowBottomRow(data);

    // Draw headers
    Tft.setFont(Terminal6x8);


    Tft.drawText(COLUMN_1, ROW_2, "Temp");
    Tft.drawText(COLUMN_2, ROW_2, "PPO2");
    Tft.drawText(COLUMN_3, ROW_2, "Head.");
    Tft.drawText(COLUMN_4, ROW_2, "Rate");

    Tft.drawText(COLUMN_1, ROW_3, "Max D");
    Tft.drawText(COLUMN_2, ROW_3, "Avg. D");
    Tft.drawText(COLUMN_3, ROW_3, "TTS");
    Tft.drawText(COLUMN_4, ROW_3, "CNS");

    // Draw Numbers
    Tft.setFont(Terminal11x16);
    // Temp
    char temp[7];
    sprintf(temp, "%4.1f", data.Temperature);
    Tft.drawText(COLUMN_1, ROW_2 + VAL_OFFSET, temp);

    // PPO2
    char ppo2[7];
    sprintf(ppo2, "%4.2f", data.PPO2);
    Tft.drawText(COLUMN_2, ROW_2 + VAL_OFFSET, ppo2);

    // heading
    char heading[7];
    sprintf(heading, "%03.0f", data.Heading);
    Tft.drawText(COLUMN_3, ROW_2 + VAL_OFFSET, heading);

    // rate
    char rate[7];
    sprintf(rate, "%.1f", data.Rate);
    Tft.drawText(COLUMN_4, ROW_2 + VAL_OFFSET, rate);

    // maxDepth
    char maxDepth[7];
    sprintf(maxDepth, "%.1f", MaxDepth);
    Tft.drawText(COLUMN_1, ROW_3 + VAL_OFFSET, maxDepth);

    // Average Depth
    char avgDepth[7];
    sprintf(avgDepth, "%.1f", data.AverageDepth);
    Tft.drawText(COLUMN_2, ROW_3 + VAL_OFFSET, avgDepth);

    // TTS
    char tts[7];
    sprintf(tts, "%02.0f", data.TTS);
    Tft.drawText(COLUMN_3, ROW_3 + VAL_OFFSET, tts);

    // CNS
    char cns[7];
    sprintf(cns, "%02.0f", data.CNS);
    Tft.drawText(COLUMN_4, ROW_3 + VAL_OFFSET, cns);
}

bool SelfTest() {
    bool pass = true;

    int textRow = 15;

    Tft.clear();
    Tft.setFont(Terminal6x8);
    Tft.drawText(10, 10, "Starting Self Test:");

    textRow += 15;
    Tft.drawText(10, textRow, "RTC...");
    if (!InitRTC()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
        char time[15];
        RealTime RTCReading = ReadRTC();
        sprintf(time, "%02d:%02d", RTCReading.Hour, RTCReading.Minute);
        Tft.drawText(150, textRow, time, COLOR_WHITE);
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
    Adc1.setGain(GAIN_TWOTHIRDS);
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "ADC2...");
    Adc2.begin();
    Adc2.setGain(GAIN_TWOTHIRDS);
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "Depth Sensor...");
    if (!InitDepth()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        DepthSensor.setModel(MS5837::MS5837_30BA);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
        char pressure[10];
        sprintf(pressure, "%.3fBar", fabs(SurfacePressure));
        Tft.drawText(150, textRow, pressure);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Deco Model...");
    Deco *testDeco = new Deco;
    float depth = 45;
    int time = 60;
    //testDeco->AddGas(0,1,0);
    testDeco->AddDecent(MeterToBar(depth), MeterToBar(testDeco->DecentRate));
    testDeco->AddBottom(time);
    std::vector<Deco::DecoStop> Schedule = GetDecoSchedule(testDeco);
    if (testDeco->Gases.empty()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }
    textRow += 15;
    Tft.drawText(10, textRow, "File System...");
    FSInfo fs_info;
    if (!InitFS(fs_info)) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
        char space[10];
        LittleFS.info(fs_info);
        sprintf(space, "%zuk/%zuk", fs_info.usedBytes / 1024, fs_info.totalBytes / 1024);
        Tft.drawText(150, textRow, space, COLOR_WHITE);
    }

    textRow += 15;
    Tft.setFont(Terminal12x16);
    if (pass) {
        Tft.drawText(10, textRow, "ALL TESTS PASSED", COLOR_GREEN);
    } else {
        Tft.drawText(10, textRow, "FAILED TESTS: HALT", COLOR_RED);
    }
    return pass;
}

void NextMenuItem() {
    CurrUIState.Menu->CurrIndex += 1;
    if (CurrUIState.Menu->CurrIndex >= CurrUIState.Menu->ItemsSize) {
        CurrUIState.InMenu = false;
        CurrUIState.Menu->CurrIndex = 0;
        CurrUIState.ClearNeeded = true;
    } else if (!CurrUIState.Menu->Items[CurrUIState.Menu->CurrIndex].Enabled) // If a menu item is disabled, skip it
    {
        NextMenuItem();
    } else {
        Tft.fillRectangle(COLUMN_1, ROW_2, COLUMN_4, ROW_3, COLOR_BLACK); // Clear prior menu item
        ShowMenu(*CurrUIState.Menu);
    }
}

void ButtonOne() {
    //Serial.println("Button 1");
    if (CurrUIState.InMenu) {
        NextMenuItem();
    } else {
        CurrUIState.InMenu = true;
        CurrUIState.ClearNeeded = true;
    }
}

void ButtonTwo() {
    //Serial.println("Button 2");
    if(CurrUIState.InMenu)
    {
        CurrUIState.Menu->Items[CurrUIState.Menu->CurrIndex].Callback(CurrUIState.Menu->Items[CurrUIState.Menu->CurrIndex]);
    }
}

void SwitchGasCallback(MenuItem item) {
    GasMenu.ItemsSize = DecoActual.Gases.size();
    for(int i=0; i < DecoActual.Gases.size() && i < GasMenu.ItemsSize; i++)
    {
        char *gas = (char*)malloc(sizeof(char)*20);
        sprintf(gas, "%02.0f/%02.0f", DecoActual.Gases[i].FrO2 * 100, DecoActual.Gases[i].FrHe * 100);

        GasMenu.Items[i].MenuText=gas;
        GasMenu.Items[i].Callback=&GasMenuCallback;
        GasMenu.Items[i].Enabled = true;
    }
    CurrUIState.Menu = &GasMenu;
    CurrUIState.ClearNeeded = true;
}

void GasMenuCallback(MenuItem item)
{
    double FrO2, FrHe;

    sscanf(item.MenuText,"%lf/%lf", &FrO2, &FrHe);

    for(int i=0; i < DecoActual.Gases.size(); i++)
    {
        if(DecoActual.Gases[i].FrO2*100 == FrO2 && DecoActual.Gases[i].FrHe == FrHe)
        {
            DecoActual.SwitchGas(i);
        }
    }

    for(int i=0; i < GasMenu.ItemsSize; i++)
    {
        free((char *) GasMenu.Items[i].MenuText);
    }

    CurrUIState.InMenu= false;
    GasMenu.CurrIndex = 0;
    CurrUIState.ClearNeeded = true;
    if(CurrUIState.Mode==DIVE)
    {
        CurrUIState.Menu=&DiveMenu;
    }
    else if (CurrUIState.Mode == SURFACE)
    {
        CurrUIState.Menu=&SurfaceMenu;
    }
}

void EndDiveCallback(MenuItem item) {
    CurrUIState.Mode = SURFACE;
    CurrUIState.ClearNeeded = true;
    CurrUIState.ClearNeeded = true;
    CurrUIState.Menu->CurrIndex=0;
    CurrUIState.Menu = &SurfaceMenu;
    CurrUIState.InMenu = false;
    EndDive();
}

void TurnOffCallback(MenuItem item) {
    TurnOff();
}

void StartWebCallback(MenuItem item) {

}

void StartDiveCallback(MenuItem item) {
    CurrUIState.Mode = DIVE;
    CurrUIState.ClearNeeded = true;
    CurrUIState.Menu->CurrIndex = 0;
    CurrUIState.Menu = &DiveMenu;
    CurrUIState.InMenu = false;
    StartDive();
}

void CalibrateCompassCallback(MenuItem item) {
    CurrUIState.InMenu = false;
    Tft.clear();
    Tft.setFont(Terminal12x16);
    Tft.drawText(COLUMN_1, ROW_1, "Compass Calibration");
    Tft.setFont(Terminal6x8);
    Tft.drawText(COLUMN_1, ROW_2, "Please spin the computer along");
    Tft.drawText(COLUMN_1, ROW_2 + 10, "all axis for 30 seconds");
    RealTime startTime = ReadRTC();
    RealTime currTime{};
    float calibrationX = 0;
    float calibrationY = 0;
    float calibrationZ = 0;
    int samples = 0;
    do {
        ResetWatchdog();
        currTime = ReadRTC();
        sensors_event_t event;
        Mag.getEvent(&event);

        calibrationX += event.magnetic.x;
        calibrationY += event.magnetic.y;
        calibrationZ += event.magnetic.z;
        samples++;

        Tft.setFont(Terminal6x8);
        char sampleCounter[5];
        sprintf(sampleCounter, "%d", samples);
        Tft.drawText(COLUMN_1, ROW_4, sampleCounter);

        Tft.setFont(Terminal12x16);
        char timeCounter[5];
        sprintf(timeCounter, "%02.0f", TimeDiff(currTime, startTime) * 60);
        Tft.drawText(COLUMN_4, ROW_4, timeCounter);
    } while (TimeDiff(currTime, startTime) < 3.0 / 6.0);

    CompassCalibration.x = calibrationX / (float) samples;
    CompassCalibration.y = calibrationY / (float) samples;
    CompassCalibration.z = calibrationZ / (float) samples;

    Settings settings = GenerateSettings(); // Save the calibration
    WriteSettingsFile(settings);

    CurrUIState.ClearNeeded = true;
    CurrUIState.InMenu = false;
}

MenuItem::MenuItem() = default;
