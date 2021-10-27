//
// Created by aren on 2/8/21.
//

#include "Screen.h"

Screen::Screen(UIElement *elements, int elementSize, int xsize, int ysize) {
    this->Elements = elements;
    this->ElementCount = elementSize;
    this->xSize = xsize;
    this->ySize = ysize;
}

void Screen::update() {
    int xInterval = this->xSize / hardwareManager.tft.maxX();
    int yInterval = this->ySize / hardwareManager.tft.maxY();

    for (int i = 0; i < this->ElementCount; i++) {
        UIElement uiElement = this->Elements[i];
        uiElement.UpdateData();
        int position = uiElement.GetPosition();
        int xIndex = position % this->xSize;
        int yIndex = position / this->ySize + 1;

        int xCoord = xIndex * xInterval;
        int yCoord = yIndex * yInterval;

        hardwareManager.tft.drawText(xCoord, yCoord, uiElement.GetTitle());
        hardwareManager.tft.drawText(xCoord, yCoord - VAL_OFFSET, uiElement.GetData());
    }
}
