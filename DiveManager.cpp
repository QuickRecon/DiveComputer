//
// Created by aren on 30/1/21.
//

#include "DiveManager.h"

Deco DecoActual;
RealTime DiveStartTime;
Deco::Schedule CurrentSchedule;
double CNSPercent = 0;
double OTUs = 0;
double AverageDepth;
double LastDepth = 0;

std::vector<Deco::DecoStop> GetDecoSchedule() {
    return GetDecoSchedule(&DecoActual);
}

std::vector<Deco::DecoStop> GetDecoSchedule(Deco *decoObject) {
    std::vector<Deco::DecoStop> Schedule;
    decoObject->FirstStopDepth = decoObject->GetCeiling();
    auto *decoSim = new Deco(*decoObject);
    while (decoSim->GetCeiling() > SurfacePressure) {
        Deco::DecoStop stop;
        if (!Schedule.empty() && Schedule.back().Gas == decoObject->BestGas(Schedule.back().Depth - 3, 1.62)) {
            stop = decoSim->GetNextDecoStop(Schedule.back().Time);
        } else {
            stop = decoSim->GetNextDecoStop();
        }
        Schedule.emplace_back(stop);
        decoSim->SwitchGas(stop.Gas);
        decoSim->AddDecent(stop.Depth, MeterToBar(-18));
        decoSim->AddBottom(stop.Time);
        ResetWatchdog();
    }
    delete decoSim;
    return Schedule;
}

double TimeDiff(RealTime time1, RealTime time2) { // Difference between 2 times, in minutes
    double minutes1 =
            time1.Second / 60.0 + time1.Minute + 60 * time1.Hour + 60 * 24 * time1.Day + 60 * 24 * 30 * time1.Month +
            60 * 24 * 365 * time1.Year; // Assume each month has 30 days, hopefully don't do multi year long dives
    double minutes2 =
            time2.Second / 60.0 + time2.Minute + 60 * time2.Hour + 60 * 24 * time2.Day + 60 * 24 * 30 * time2.Month +
            60 * 24 * 365 * time2.Year;

    return minutes1 - minutes2;
}

Deco::Gas GetCurrGas() {
    return DecoActual.Gases[DecoActual.CurrentGas];
}

void AddO2Exposure(double PPO2, double time) {

}

double GetTTS(const std::vector<Deco::DecoStop> &schedule) {
    double tts = 0;
    double lastDepth = LastDepth;
    if (!schedule.empty()) {
        for (auto &i : schedule) {
            tts += (LastDepth - i.Depth) / ASCENT_RATE; // Add time to ascent to stop
            tts += i.Time; //Add stop time
        }
    } else {
        tts = LastDepth / ASCENT_RATE;
    }
    return tts;
}