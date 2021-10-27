//
// Created by aren on 2/8/21.
//

#ifndef DIVECOMPUTER_RTC_H
#define DIVECOMPUTER_RTC_H

#include <SparkFunDS3234RTC.h>

struct RealTime {
    int Second;
    int Minute;
    int Hour;

    int Day;
    int Month;
    int Year;
};

#define RTC_CS 0
#define SET_CLOCK 0

class RTC {
public:
    RTC();

    RealTime ReadRTC();

    bool check();

private:
    bool ready;
};


#endif //DIVECOMPUTER_RTC_H
