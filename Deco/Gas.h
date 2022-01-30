//
// Created by aren on 28/10/21.
//

#ifndef DIVECOMPUTER_GAS_H
#define DIVECOMPUTER_GAS_H

#include "Utils/ErrorHandler.h"
#include <cstdio>
#include <cstring>
#include <vector>

class Gas {
public:
    Gas(float oxygen, float helium);

    Gas() : Gas(0.21, 0) {};

    float getOxygen() const;

    float getHelium() const;

    float getNitrogen() const;

    char *toString(char gas[6]) const;

private:
    float Oxygen;
    float Helium;
};

std::vector<Gas> *GasList = new std::vector<Gas>();

#endif //DIVECOMPUTER_GAS_H
