//
// Created by aren on 30/1/21.
//

#include "DiveManager.h"

Deco DecoActual;
RealTime DiveStartTime;

std::vector <Deco::DecoStop> GetDecoSchedule() {
    return GetDecoSchedule(&DecoActual);
}

std::vector <Deco::DecoStop> GetDecoSchedule(Deco *decoObject) {
    std::vector <Deco::DecoStop> Schedule;
    decoObject->FirstStopDepth = decoObject->GetCeiling();
    auto *decoSim = new Deco(*decoObject);
    while (decoSim->GetCeiling() > 1.031) {
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
