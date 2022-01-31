//
// Created by aren on 30/1/22.
//

#ifndef DIVECOMPUTER_DRAWER_H
#define DIVECOMPUTER_DRAWER_H

#include <string>
#include "UIElement.h"
#include <Hardware/HardwareManager.h>

class Drawer {
public:
    void DrawMenu(std::string menuText);

    void ClearMenu();

    void DrawUIElement(int xCoord, int yCoord, UIElement *uiElement);

private:
    int MenuHeight;
    int ValueOffset;

};


#endif //DIVECOMPUTER_DRAWER_H
