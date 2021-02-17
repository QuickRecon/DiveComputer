//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_UI_H
#define DIVECOMPUTER_UI_H

#include "HWInterface.h"
#include "DiveManager.h"
#include "FSInterface.h"

#define ROW_1 10
#define ROW_2 50
#define ROW_3 90
#define ROW_4 130

#define VAL_OFFSET 15

#define COLUMN_1 10
#define COLUMN_2 60
#define COLUMN_3 110
#define COLUMN_4 160

#define SURFACE_DIVE_THRESHOLD 0.5

#define END_DIVE_TIMEOUT 5
#define TURN_OFF_TIMEOUT 10

#define MAX_GAS_COUNT 10
enum Mode {
    DIVE,
    SURFACE,
    WIFI
};

struct MenuItem {
    MenuItem();

    const char *MenuText{};

    void ( *Callback)(MenuItem &item){};
    bool Enabled{};

    MenuItem(const char *menuText, void (*const callback)(MenuItem &item), bool enabled) : MenuText(menuText),
                                                                                           Callback(callback),
                                                                                           Enabled(enabled) {}
};

struct Menu
{
    struct MenuItem **Items;
    int ItemsSize;
    int CurrIndex;

    Menu(struct MenuItem **items, int itemsSize, int currIndex) : Items(items), ItemsSize(itemsSize),
                                                                  CurrIndex(currIndex) {}
};

struct UIState {
    int Mode;
    bool InMenu;
    struct Menu *Menu;
    bool ClearNeeded;
};
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

extern UIState CurrUIState;

bool SelfTest();

void ShowDiveScreen(UIData &data);

void ShowSurfaceScreen(UIData &data);

void ButtonOne();

void ButtonTwo();

void ShowDiveTopRow(UIData &data);

void ShowBottomRow(UIData &data);

void ShowBar(Bar &bar, double value);

void UpdateUI(UIData &data);

void ShowMenu(Menu &menu);

void SwitchGasCallback(MenuItem &item);

void EndDiveCallback(MenuItem &item);

void TurnOffCallback(MenuItem &item);

void StartWebCallback(MenuItem &item);

void StartDiveCallback(MenuItem &item);

void GasMenuCallback(MenuItem &item);

void CalibrateCompassCallback(MenuItem &item);

#endif //DIVECOMPUTER_UI_H
