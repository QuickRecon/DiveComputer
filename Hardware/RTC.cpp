//
// Created by aren on 2/8/21.
//

#include "RTC.h"

RTC::RTC() {
    rtc.begin(RTC_CS);
    if (SET_CLOCK) {
        rtc.autoTime();
    }
    this->ready = ReadRTC().Second <= 60 && ReadRTC().Minute <= 60 && ReadRTC().Hour <= 24;
}

RealTime RTC::ReadRTC() {
    rtc.update();
    RealTime data{};

    data.Day = rtc.date();
    data.Month = rtc.month();
    data.Year = rtc.year();

    data.Hour = rtc.hour();
    data.Minute = rtc.minute();
    data.Second = rtc.second();

    return data;
}

bool RTC::check() {
    return this->ready;
}


