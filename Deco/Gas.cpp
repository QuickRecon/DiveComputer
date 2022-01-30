//
// Created by aren on 28/10/21.
//

#include "Gas.h"

Gas::Gas(float oxygen, float helium) {
    if (oxygen + helium > 1.0) {
        char gas[6];
        char message[22] = "Gas ";
        sprintf(gas, "%02.0f/%02.0f", oxygen * 100, helium * 100);
        strcat(message, gas);
        strcat(message, " is not valid.");

        errorHandler.Error(message);
    }

    this->Oxygen = oxygen;
    this->Helium = helium;
}

float Gas::getOxygen() const {
    return this->Oxygen;
}

float Gas::getHelium() const {
    return this->Helium;
}

float Gas::getNitrogen() const {
    return 1 - this->Oxygen - this->Helium;
}

char *Gas::toString(char gas[6]) const {
    sprintf(gas, "%02.0f/%02.0f", this->Oxygen, this->Helium);
    return gas;
}

