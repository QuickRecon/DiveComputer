//
// Created by aren on 30/1/21.
//
#include "UI.h"

bool ShowingDiveScreen = false;

void ShowDiveScreen(DiveScreen data)
{
    if(!ShowingDiveScreen)
    {
        Tft.clear();
    }
    ShowingDiveScreen = true;

    // Draw headers
    Tft.setFont(Terminal6x8);
    Tft.drawText(COLUMN_1, ROW_1, "Depth");
    Tft.drawText(COLUMN_2, ROW_1, "Time");
    if(data.NDL == -1)
    {
        Tft.drawText(COLUMN_3, ROW_1, "Stop");
        Tft.drawText(COLUMN_4, ROW_1, "S Time");
    }
    else
    {
        Tft.drawText((COLUMN_3+COLUMN_4)/2, ROW_1, "NDL");
    }

    Tft.drawText(COLUMN_2, ROW_2, "PPO2");
    Tft.drawText(COLUMN_3, ROW_2, "Head.");
    Tft.drawText(COLUMN_4, ROW_2, "Rate");


    Tft.drawText(COLUMN_2, ROW_3, "Avg. D");
    Tft.drawText(COLUMN_3, ROW_3, "TTS");
    Tft.drawText(COLUMN_4, ROW_3, "CNS");

    Tft.drawText((COLUMN_1+COLUMN_2)/2, ROW_4, "Gas");
    Tft.drawText((COLUMN_3+COLUMN_4)/2, ROW_4, "Clock");

    // Draw Numbers
    Tft.setFont(Terminal11x16);
    // Depth
    char depth[7];
    sprintf(depth, "%04.1f",fabs(data.Depth));
    Tft.drawText(COLUMN_1, ROW_1+VAL_OFFSET, depth);

    // Dive Time
    char diveTime[7];
    sprintf(diveTime, "%3.0f",floor(fabs(data.DiveTime)));
    Tft.drawText(COLUMN_2, ROW_1+VAL_OFFSET, diveTime);

    if(data.NDL == -1)
    {
        char stop[4];
        sprintf(stop, "%.0f",data.Stop.Depth);
        char stime[4];
        sprintf(stime, "%.0f",data.Stop.Time);
        Tft.drawText(COLUMN_3, ROW_1+VAL_OFFSET, stop);
        Tft.drawText(COLUMN_4, ROW_1+VAL_OFFSET, stime);
    }
    else
    {
        char ndl[4];
        sprintf(ndl, "%.0f",data.NDL);
        Tft.drawText((COLUMN_3+COLUMN_4)/2, ROW_1+VAL_OFFSET, ndl);
    }



    // PPO2
    char ppo2[7];
    sprintf(ppo2, "%4.2f",data.PPO2);
    Tft.drawText(COLUMN_2, ROW_2+VAL_OFFSET, ppo2);

    // heading
    char heading[7];
    sprintf(heading, "%03.0f",data.Heading);
    Tft.drawText(COLUMN_3, ROW_2+VAL_OFFSET, heading);

    // rate
    char rate[7];
    sprintf(rate, "%.1f",data.Rate);
    Tft.drawText(COLUMN_4, ROW_2+VAL_OFFSET, rate);

    // Average Depth
    char avgDepth[7];
    sprintf(avgDepth, "%.1f",data.AverageDepth);
    Tft.drawText(COLUMN_2, ROW_3+VAL_OFFSET, avgDepth);

    // TTS
    char tts[7];
    sprintf(tts, "%.0f",data.TTS);
    Tft.drawText(COLUMN_3, ROW_3+VAL_OFFSET, tts);

    // CNS
    char cns[7];
    sprintf(cns, "%.0f",data.CNS);
    Tft.drawText(COLUMN_4, ROW_3+VAL_OFFSET, cns);

    // Gas
    char gas[7];
    sprintf(gas, "%02.0f/%02.0f",data.Gas.FrO2*100,data.Gas.FrHe*100);
    Tft.drawText(COLUMN_1+10, ROW_4+VAL_OFFSET, gas);

    // Real Time
    char time[7];
    sprintf(time, "%02d:%02d",data.Time.Hour, data.Time.Minute);
    Tft.drawText(COLUMN_3+10, ROW_4+VAL_OFFSET, time);
}


bool SelfTest() {
    bool pass = true;

    int textRow = 15;

    Tft.clear();
    Tft.drawText(10, 10, "Starting Self Test:");

    textRow += 15;
    Tft.drawText(10, textRow, "RTC...");
    if (!InitRTC()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Accelerometer...");
    if (!Accel.begin()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Accel.setRange(LSM303_RANGE_4G);
        Accel.setMode(LSM303_MODE_NORMAL);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Magnetometer...");
    if (!Mag.begin()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Mag.enableAutoRange(true);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "ADC1...");
    Adc1.begin();
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "ADC2...");
    Adc2.begin();
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "Depth Sensor...");
    if (!InitDepth()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        DepthSensor.setModel(MS5837::MS5837_30BA);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Deco Model...");
    Deco *testDeco = new Deco;
    float depth = 60;
    int time = 60;
    //testDeco->AddGas(0,1,0);
    testDeco->AddDecent(MeterToBar(depth), MeterToBar(testDeco->DecentRate));
    testDeco->AddBottom(time);
    std::vector <Deco::DecoStop> Schedule = GetDecoSchedule(testDeco);
    if (testDeco->Gases.empty()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
    }

    textRow += 25;
    Tft.setFont(Terminal12x16);
    if (pass) {
        Tft.drawText(10, textRow, "ALL TESTS PASSED", COLOR_GREEN);
    } else {
        Tft.drawText(10, textRow, "FAILED TESTS: HALT", COLOR_RED);
    }
    return pass;
}