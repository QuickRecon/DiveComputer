//
// Created by aren on 28/10/21.
//

#ifndef DIVECOMPUTER_GASMENU_H
#define DIVECOMPUTER_GASMENU_H

#include "UI/Menu.h"
#include "Deco/Gas.h"

Menu GasMenu = Menu({}, 0, 200);

void SwitchGasCallback(MenuItem &item);

void GasMenuCallback(MenuItem &item);

#endif //DIVECOMPUTER_GASMENU_H
