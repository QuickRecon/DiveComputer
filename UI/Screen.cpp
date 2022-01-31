//
// Created by aren on 2/8/21.
//

#include "Screen.h"

Screen::Screen(std::vector<UIElement> *elements, int xsize, int ysize, int screenX, int screenY) {
    if (xsize * ysize != elements->size()) {
        throw "Elements does not match screen dimensions";
    }

    this->Elements = elements;
    this->xSize = xsize;
    this->ySize = ysize;
    this->ScreenX = screenX;
    this->ScreenY = screenY;
}

void Screen::update() {
    int xInterval = this->ScreenX / this->xSize;
    int yInterval = this->ScreenY / this->ySize;

    for (auto uiElement : *this->Elements) {
        uiElement.UpdateData();
        int position = uiElement.GetPosition();
        int xIndex = position % this->xSize;
        int yIndex = position / this->ySize;

        int xCoord = xIndex * xInterval;
        int yCoord = yIndex * yInterval;

        uiElement.Draw(xCoord, yCoord);
    }
}
