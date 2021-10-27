//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_SCREEN_H
#define DIVECOMPUTER_SCREEN_H

#include "UIElement.h"
#include "Hardware/HardwareManager.h"

#define VAL_OFFSET 15

class Screen {
public:
    Screen() = delete;

    Screen(UIElement *elements, int elementSize, int xSize, int ySize);

    void update();

private:
    int ElementCount;
    int xSize;
    int ySize;
    UIElement Elements[];
};


#endif //DIVECOMPUTER_SCREEN_H
