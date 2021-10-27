//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_MODE_H
#define DIVECOMPUTER_MODE_H

#include "Screen.h"
#include "Menu.h"

class Mode {
public:
    Mode() = delete;

    Mode(Screen *(*screens), int screenCount, class Menu *menu);

    void update();

private:
    Screen *CurrScreen;
    Menu *Menu;
    Screen *(*Screens);

    int ScreenCount;
};


#endif //DIVECOMPUTER_MODE_H
