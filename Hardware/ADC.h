//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_ADC_H
#define DIVECOMPUTER_ADC_H

#include <QR_ADS1115.h>
#include <map>

class ADC {
public:
    ADC(int address, adsGain_t gain);

    ADC() : ADC(0x48, GAIN_SIXTEEN) {};

    double ReadValue(int channel);

    double ReadVoltage(int channel, bool millivolts);

    bool check();

private:
    QR_ADS1115 sensor;
    adsGain_t gain;
    std::map<adsGain_t, double> GainToMillivoltPerBit{
            {GAIN_TWOTHIRDS, 0.1875},
            {GAIN_ONE,       0.125},
            {GAIN_TWO,       0.0625},
            {GAIN_FOUR,      0.03125},
            {GAIN_EIGHT,     0.015625},
            {GAIN_SIXTEEN,   0.0078125}
    };
    bool ready;
};


#endif //DIVECOMPUTER_ADC_H
