//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_IO_H
#define DIVECOMPUTER_IO_H

#include <Wire.h>
#include <Arduino.h>
#include "PCF8574.h"

//#define IO_ADDRESS 0x20

// Reset circuit parameters
#define LATCH_ENABLE_RESET_ENABLE   0b11111111
#define LATCH_DISABLE_RESET_ENABLE  0b11111001
#define LATCH_ENABLE_RESET_DISABLE  0b11111110
#define LATCH_DISABLE_RESET_DISABLE 0b11111000

// Button Pins
#define BUTTON_1_PIN P6
#define BUTTON_2_PIN P7

class IO {
public:
    IO(int address, int button1Pin, int button2Pin);

    IO() : IO(0x20, BUTTON_1_PIN, BUTTON_2_PIN) {};

    void PollButtons();

    void EnableReset();

    void DisableReset();

    bool check();

private:
    bool ready;
    int address;

    int Button1Pin;
    int Button2Pin;

    void (*Button1Callback)();

    void (*Button2Callback)();
};


#endif //DIVECOMPUTER_IO_H
