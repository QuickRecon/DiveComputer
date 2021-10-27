//
// Created by aren on 2/8/21.
//


#include "IO.h"

IO::IO(int address, int button1Pin, int button2Pin) {
    this->address = address;
    this->Button1Pin = button1Pin;
    this->Button2Pin = button2Pin;
    DisableReset();

    Wire.beginTransmission(this->address);
    this->ready = Wire.endTransmission() == 0;
}

void IO::PollButtons() {
    Wire.requestFrom((uint8_t) this->address, (uint8_t) 1);
    byte IOByte = Wire.read();// Read a byte

    Serial.println(IOByte);

    bool button1 = (bit(this->Button1Pin) & IOByte) == 0;
    bool button2 = (bit(this->Button2Pin) & IOByte) == 0;

    Serial.print("Button 1: ");
    Serial.println(button1);
    Serial.print("Button 2: ");
    Serial.println(button2);

    if (button1 && button2) {
        // Implement 2 button combination?
    } else if (button1) {
        (*Button1Callback)();
    } else if (button2) {
        (*Button2Callback)();
    }
}

void IO::EnableReset() {
    Wire.beginTransmission(this->address); // Plus 1 to switch to write address
    Wire.write(LATCH_ENABLE_RESET_ENABLE);
    delay(100); // Delay a tad to allow the circuits to set up
    Wire.write(LATCH_DISABLE_RESET_ENABLE);
    Wire.endTransmission();
}

void IO::DisableReset() {
    Wire.beginTransmission(this->address);
    Wire.write(LATCH_ENABLE_RESET_DISABLE);
    delay(100); // Delay a tad to allow the circuits to set up
    Wire.write(LATCH_DISABLE_RESET_DISABLE);
    Wire.endTransmission();
}

bool IO::check() {
    return ready;
}

