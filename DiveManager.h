//
// Created by aren on 30/1/21.
//

#ifndef DIVECOMPUTER_DIVEMANAGER_H
#define DIVECOMPUTER_DIVEMANAGER_H

#include <Deco.h>
#include "DiveComputer.h"
#include "HWInterface.h"

#define ASCENT_RATE 10
#define DESCENT_RATE 30

struct RealTime;

extern double CNSPercent;
extern double OTUs;
extern double AverageDepth;
extern double LastDepth;
extern Deco DecoActual;
extern RealTime DiveStartTime;
extern Deco::Schedule CurrentSchedule;

Deco::Schedule GetDecoSchedule();

Deco::Schedule GetDecoSchedule(Deco *decoObject);

double TimeDiff(RealTime time1, RealTime time2);

Deco::Gas GetCurrGas();

double GetTTS(const Deco::Schedule &schedule);

void StartDive();

void EndDive();

#endif //DIVECOMPUTER_DIVEMANAGER_H