//
// Created by aren on 30/1/21.
//

#include "DiveManager.h"

Deco DecoActual;
RealTime DiveStartTime;
Deco::Schedule CurrentSchedule;
double CNS = 0;
double OTUs = 0;
double AverageDepth = 0;
double DepthSamples = 0;
double DepthSum = 0;
double MaxDepth = 0;
double LastUpdateTime = 0;
double LastDiveDepth = 0;
double LastDiveTime = 0;
double LastDepth = 0;
double Rate = 0;
double CNSSlopes[] = {-1800, -1500, -1200, -900, -600, -300, -750};
double CNSIntercepts[] = {1800, 1620, 1410, 1170, 900, 570, 1245};

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

void UpdateAvgDepth(double depth) {
    if (depth > SURFACE_DIVE_THRESHOLD) { // Only add to average depth if its not at the surface
        DepthSum += depth;
        DepthSamples++;
        AverageDepth = (DepthSum) / (DepthSamples);
    }

}

void UpdateRate(double depth) {
    Rate = depth - LastDepth;
}

void AddO2Exposure(double PPO2, double time) { // Use rectangular approximation
    if (PPO2 > 0.5) { // oxtox only applies for PPO2 > 0.5
        // Calculate OTUs
        OTUs += time * pow(0.5 / (PPO2 - 0.5), -5.0 / 3.0);

        // CNS
        int CNSrange;
        if (PPO2 > 0.5 && PPO2 <= 0.6) { CNSrange = 0; }
        else if (PPO2 > 0.6 && PPO2 <= 0.7) { CNSrange=1; }
        else if (PPO2 > 0.7 && PPO2 <= 0.8) { CNSrange=2; }
        else if (PPO2 > 0.8 && PPO2 <= 0.9) { CNSrange = 3; }
        else if (PPO2 > 0.9 && PPO2 <= 1.1) { CNSrange = 4; }
        else if (PPO2 > 1.1 && PPO2 <= 1.5) { CNSrange = 5; }
        else if (PPO2 > 1.5 && PPO2 <= 1.6) { CNSrange = 6; }
        else { CNSrange = 6; } // Exceeding limits

        double slope = CNSSlopes[CNSrange];
        double intercept = CNSIntercepts[CNSrange];

        CNS += time / (slope * PPO2 + intercept);
    }
}

void UpdateMaxDepth(double depth) {
    if (depth > MaxDepth) {
        MaxDepth = depth;
    }
}

void AddDiveSegment(UIData data, double time) {
    UpdateAvgDepth(data.Depth);
    UpdateMaxDepth(data.Depth);
    UpdateRate(data.Depth);
    DecoActual.AddDecent(data.AmbientPressure, time);
    AddO2Exposure(data.PPO2, time);
    EnterInDiveLog(data);
    LastUpdateTime = data.DiveTime;
    LastDepth = data.Depth;
}

void UpdateDiveManager(UIData data) {
    double time = data.DiveTime - LastUpdateTime;
    if (time > ALGO_UPDATE_RATE) {
        if (CurrUIState.Mode == DIVE) {
            AddDiveSegment(data, time);
        } else {
            DecoActual.AddDecent(data.AmbientPressure, time);
            DecayO2Exposure(time);
            LastUpdateTime = data.DiveTime;
            LastDepth = data.Depth;
        }

    }
}

double GetTTS(const std::vector<Deco::DecoStop> &schedule) {
    double tts = 0;
    if (!schedule.empty()) {
        for (auto &i : schedule) {
            tts += (LastDepth - i.Depth) / ASCENT_RATE; // Add Time to ascend to stop
            tts += i.Time; //Add stop Time
        }
    } else {
        tts = LastDepth / ASCENT_RATE;
    }
    return tts;
}

void StartDive() {
    DiveStartTime = ReadRTC();
    AverageDepth = 0;
    MaxDepth = 0;
    DepthSum = 0;
    DepthSamples = 0;
    LastUpdateTime = 0;
    OpenDiveLog();
}

void EndDive() {
    CloseDiveLog();
    LastDiveDepth = MaxDepth;
    LastDiveTime = TimeDiff(ReadRTC(), DiveStartTime);
    Settings settings = GenerateSettings();
    WriteSettingsFile(settings);
}

void DecayO2Exposure(double time) {
    CNS = CNS * pow(0.5, time / 90); // Decay CNS with a halflife of 90 minutes
}