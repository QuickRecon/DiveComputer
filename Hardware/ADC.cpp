//
// Created by aren on 2/8/21.
//

#include "ADC.h"

ADC::ADC(int address, adsGain_t gain) {
    this->gain = gain;

    sensor = QR_ADS1115(address);

    sensor.begin();
    sensor.setGain(gain);
    sensor.getLastConversionResults();

    Wire.beginTransmission(address);
    this->ready = Wire.endTransmission() == 0;
}

double ADC::ReadValue(int channel) {
    return sensor.readADC_SingleEnded(channel);
}

double ADC::ReadVoltage(int channel, bool millivolts) {
    double value = this->ReadValue(channel);
    double voltage = GainToMillivoltPerBit[gain] * value;
    if (!millivolts) {
        voltage *= 1000;
    }
    return voltage;
}

bool ADC::check() {
    return ready;
}

