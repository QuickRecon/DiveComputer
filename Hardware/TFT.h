//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_TFT_H
#define DIVECOMPUTER_TFT_H

#include "TFT_22_ILI9225.h"
#include <Arduino.h>

// TFT Params
#define TFT_RST 0   // NULL
#define TFT_RS  2   // D0
#define TFT_CS  15  // D8 SS
#define TFT_LED 16  // D4     set 0 if wired to +5V directly -> D3=0 is not possible !!


class TFT {
public:
    TFT(int8_t resetPin, int8_t registerPin, int8_t chipPin, int ledPin);

    TFT() : TFT(TFT_RST, TFT_RS, TFT_CS, TFT_LED) {};

    void setFont(uint8_t *font);

    void drawText(uint16_t x, uint16_t y, STRING s, uint16_t color = COLOR_WHITE);

    void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    uint16_t maxX();

    uint16_t maxY();

    void off();

private:
    TFT_22_ILI9225 tft;
    int LEDPin;
};


#endif //DIVECOMPUTER_TFT_H
