//
// Created by aren on 2/8/21.
//

#include "Mode.h"

Mode::Mode(Screen *(*screens), int screenCount, class Menu *menu) {
    this->Screens = screens;
    this->ScreenCount = screenCount;

    this->Menu = menu;

    this->CurrScreen = this->Screens[0];
}

void Mode::update() {

}