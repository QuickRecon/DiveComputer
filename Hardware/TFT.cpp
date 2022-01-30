//
// Created by aren on 2/8/21.
//

#include "TFT.h"

TFT::TFT(int8_t resetPin, int8_t registerPin, int8_t chipPin, int ledPin) {
    this->LEDPin = ledPin;
    this->tft = TFT_22_ILI9225(resetPin, registerPin, chipPin, 0, 255);
    tft.begin();
    tft.clear();
    tft.setOrientation(3);
}

void TFT::off() {
    // Turn off display
    digitalWrite(this->LEDPin, LOW);
    tft.setDisplay(false);
}

void TFT::setFont(uint8_t *font) {
    this->tft.setFont(font);
}

void TFT::drawText(uint16_t x, uint16_t y, String s, uint16_t color) {
    this->tft.drawText(x, y, s, color);
}

void TFT::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    this->tft.fillRectangle(x1, y1, x2, y2, color);
}

uint16_t TFT::maxX() {
    return this->tft.maxX();
}

uint16_t TFT::maxY() {
    return this->tft.maxY();
}

