//
// Created by aren on 28/10/21.
//

#include "SurfaceMode.h"


void TurnOffCallback(MenuItem &item) {

}

void StartWebCallback(MenuItem &item) {

}

void StartDiveCallback(MenuItem &item) {

}

void CalibrateCompassCallback(MenuItem &item) {

}

void SwitchGasCallback(MenuItem &item) {

}

SurfaceMode::SurfaceMode() {
    // Menu Items
    MenuItem MenuSwitchGas = MenuItem("Switch Gas", &SwitchGasCallback);
    MenuItem MenuTurnOff = MenuItem("Turn Off", &TurnOffCallback);
    MenuItem MenuStartWeb = MenuItem("Start Web", &StartWebCallback);
    MenuItem MenuStartDive = MenuItem("Start Dive", &StartDiveCallback);
    MenuItem CalibrateCompass = MenuItem("Calibrate Compass", &CalibrateCompassCallback);

#ifndef DEBUG
    MenuStartDive.Disable();
#endif

    // Menu Structure
    MenuItem *SurfaceItems[] = {&MenuSwitchGas, &MenuStartDive, &MenuStartWeb, &CalibrateCompass, &MenuTurnOff};

    Menu SurfaceMenu = Menu(SurfaceItems, 5, 0);

    // Screen

}
