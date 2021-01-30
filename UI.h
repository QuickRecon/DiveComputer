//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_UI_H
#define DIVECOMPUTER_UI_H

#include "DiveManager.h"
#include "HWInterface.h"

struct DiveScreen
{
    double Depth;
    double AmbientPressure;
    double Temperature;
    double DiveTime;
    Deco::DecoStop Stop;
    Deco::Gas Gas;
    double PPO2;
    double Heading;
    double Rate;
    double Time;
    double AverageDepth;
    int TTS;
    int CNS;

};

bool SelfTest();

#endif //DIVECOMPUTER_UI_H
