//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_SCREEN_H
#define DIVECOMPUTER_SCREEN_H

#include <vector>
#include "UIElement.h"

#define VAL_OFFSET 15

class Screen {
public:
    Screen() = delete;

    Screen(std::vector<UIElement> *elements, int xsize, int ysize, int screenX, int screenY);

    void update();

private:
    int ScreenX;
    int ScreenY;
    int xSize;
    int ySize;
    std::vector<UIElement> *Elements;
};


#endif //DIVECOMPUTER_SCREEN_H
