//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_DIVEMANAGER_H
#define DIVECOMPUTER_DIVEMANAGER_H
#include <Deco.h>
#include "DiveComputer.h"
#include "HWInterface.h"

struct RealTime
{
    int Second;
    int Minute;
    int Hour;

    int Day;
    int Month;
    int Year;
};

extern Deco DecoActual;

extern RealTime DiveStartTime;

std::vector <Deco::DecoStop> GetDecoSchedule();
std::vector <Deco::DecoStop> GetDecoSchedule(Deco *decoObject);
#endif //DIVECOMPUTER_DIVEMANAGER_H