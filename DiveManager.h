//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_DIVEMANAGER_H
#define DIVECOMPUTER_DIVEMANAGER_H

#include <Deco.h>
#include "DiveComputer.h"
#include "HWInterface.h"
#include "UI.h"
#include "FSInterface.h"

#define ASCENT_RATE 10
#define DESCENT_RATE 30

#define ALGO_UPDATE_RATE 0.1 // Update every 6 seconds

extern double CNSSlopes[];
extern double CNSIntercepts[];


struct RealTime;
struct UIData;

extern double CNS;
extern double OTUs;
extern double AverageDepth;
extern double DepthSum;
extern double MaxDepth;
extern double LastDepth;
extern Deco DecoActual;
extern RealTime DiveStartTime;
extern Deco::Schedule CurrentSchedule;
extern double LastUpdateTime;
extern double LastDiveDepth;
extern double LastDiveTime;

Deco::Schedule GetDecoSchedule();

Deco::Schedule GetDecoSchedule(Deco *decoObject);

double TimeDiff(RealTime time1, RealTime time2);

Deco::Gas GetCurrGas();

double GetTTS(const Deco::Schedule &schedule);

void StartDive();

void EndDive();

void UpdateDiveManager(UIData data);

void DecayO2Exposure(double time);

#endif //DIVECOMPUTER_DIVEMANAGER_H