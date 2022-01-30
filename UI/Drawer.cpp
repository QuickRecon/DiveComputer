//
// Created by aren on 30/1/22.
//

#include <Hardware/HardwareManager.h>
#include "Drawer.h"

void Drawer::DrawMenu(std::string menuText) {
    hardwareManager.tft.setFont(Terminal12x16);
    hardwareManager.tft.drawText(0, hardwareManager.tft.maxY() - this->MenuHeight,
                                 menuText);
}

void Drawer::ClearMenu() {
    hardwareManager.tft.fillRectangle(0, hardwareManager.tft.maxY() - this->MenuHeight,
                                      hardwareManager.tft.maxX(), hardwareManager.tft.maxY(),
                                      COLOR_BLACK); // Clear prior menu item
}
