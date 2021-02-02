//
// Created by aren on 30/1/21.
//
#include "UI.h"

bool ShowingDiveScreen = false;
bool ShowingSurfaceScreen = false;

Bar DiveTimeBar = {.OriginX=COLUMN_2,.OriginY=ROW_1+10,.Length=28,.Thickness=2,.Vertical=false,.Color=COLOR_WHITE,.MaxValue=1,.EndStop=true};
Bar TimeBar = {.OriginX=COLUMN_4+18,.OriginY=ROW_4+VAL_OFFSET+15,.Length=15,.Thickness=2,.Vertical=true,.Color=COLOR_WHITE,.MaxValue=60,.EndStop=true};

void ShowBar(Bar bar, double value)
{
    int EndX, EndY,ValX,ValY;
    if(bar.Vertical)
    {
        ValX=bar.OriginX+bar.Thickness;
        ValY=bar.OriginY-(int)ceil(value/bar.MaxValue * bar.Length);

        EndX=bar.OriginX+bar.Thickness;
        EndY=bar.OriginY-bar.Length;
    }
    else
    {
        ValY=bar.OriginY+bar.Thickness;
        ValX=bar.OriginX+ (int)ceil(value/bar.MaxValue * bar.Length);

        EndY=bar.OriginY+bar.Thickness;
        EndX=bar.OriginX+bar.Length;
    }
    Tft.fillRectangle(bar.OriginX,bar.OriginY,ValX,ValY,bar.Color);
    if((int)ceil(value/bar.MaxValue * bar.Length) != bar.Length)
    {
        Tft.fillRectangle(ValX-bar.Thickness,ValY-bar.Thickness,EndX,EndY,COLOR_BLACK); // Blank Out any part of the bar that isn't displayed
    }
    if(bar.EndStop)
    {
        if(bar.Vertical)
        {
            Tft.fillRectangle(EndX-bar.Thickness, EndY-1,EndX,EndY-1,bar.Color);
        }
        else
        {
            Tft.fillRectangle(EndX-1, EndY-bar.Thickness,EndX-1,EndY,bar.Color);
        }
    }
}

void ShowDiveTopRow(UIData data)
{
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

    Tft.setFont(Terminal11x16);
    // Depth
    char depth[7];
    sprintf(depth, "%04.1f",fabs(data.Depth));
    Tft.drawText(COLUMN_1, ROW_1+VAL_OFFSET, depth);

    // Dive Time
    char diveTime[7];
    sprintf(diveTime, "%3.0f",floor(fabs(data.DiveTime)));
    Tft.drawText(COLUMN_2, ROW_1+VAL_OFFSET, diveTime);
    ShowBar(DiveTimeBar, data.DiveTime - (double) ((int) data.DiveTime));

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

    Tft.drawRectangle(0,ROW_1+VAL_OFFSET+18,Tft.maxX(),ROW_1+VAL_OFFSET+18, COLOR_WHITE);
}

void ShowBottomRow(UIData data){
    Tft.setFont(Terminal6x8);

    Tft.drawText((COLUMN_1+COLUMN_2)/2, ROW_4, "Gas");
    Tft.drawText((COLUMN_3+COLUMN_4)/2, ROW_4, "Clock");

    Tft.setFont(Terminal11x16);

    // Gas
    char gas[7];
    sprintf(gas, "%02.0f/%02.0f",data.Gas.FrO2*100,data.Gas.FrHe*100);
    Tft.drawText(COLUMN_1+10, ROW_4+VAL_OFFSET, gas);

    // Real Time
    char time[7];
    sprintf(time, "%02d:%02d",data.Time.Hour, data.Time.Minute);
    Tft.drawText(COLUMN_3+10, ROW_4+VAL_OFFSET, time);
    ShowBar(TimeBar, data.Time.Second);

    Tft.drawRectangle(0,ROW_4-3,Tft.maxX(),ROW_4-3, COLOR_WHITE);
}

void ShowSurfaceScreen(UIData data)
{
    if(!ShowingSurfaceScreen)
    {
        Tft.clear();
    }
    ShowingSurfaceScreen = true;
    ShowingDiveScreen = false;
    ShowBottomRow(data);
    Tft.setFont(Terminal6x8);
    Tft.drawText(COLUMN_1, ROW_1, "Surface Pressure");
    Tft.drawText(COLUMN_3+20, ROW_1, "Temperature");

    Tft.drawText((COLUMN_1+COLUMN_2)/2-10, ROW_2, "GFL/GFH");
    Tft.drawText((COLUMN_1+COLUMN_2)/2, ROW_3, "CNS");


    Tft.setFont(Terminal11x16);
    char pressure[10];
    sprintf(pressure, "%.3fBar",fabs(data.AmbientPressure));
    Tft.drawText(COLUMN_1, ROW_1+VAL_OFFSET, pressure);

    char temperature[10];
    sprintf(temperature, "%02.1fC",data.Temperature);
    Tft.drawText(COLUMN_3+25, ROW_1+VAL_OFFSET, temperature);

    char gradientFactors[10];
    sprintf(gradientFactors, "%02.0f/%02.0f",DecoActual.GFLow*100,DecoActual.GFHigh*100);
    Tft.drawText(COLUMN_1+10, ROW_2+VAL_OFFSET, gradientFactors);

    char cns[7];
    sprintf(cns, "%02.0f",data.CNS);
    Tft.drawText((COLUMN_1+COLUMN_2)/2, ROW_3+VAL_OFFSET, cns);
}

void ShowDiveScreen(UIData data)
{
    if(!ShowingDiveScreen)
    {
        Tft.clear();
    }
    ShowingDiveScreen = true;
    ShowingSurfaceScreen = false;

    ShowDiveTopRow(data);
    ShowBottomRow(data);

    // Draw headers
    Tft.setFont(Terminal6x8);


    Tft.drawText(COLUMN_1, ROW_2, "Temp");
    Tft.drawText(COLUMN_2, ROW_2, "PPO2");
    Tft.drawText(COLUMN_3, ROW_2, "Head.");
    Tft.drawText(COLUMN_4, ROW_2, "Rate");


    Tft.drawText(COLUMN_2, ROW_3, "Avg. D");
    Tft.drawText(COLUMN_3, ROW_3, "TTS");
    Tft.drawText(COLUMN_4, ROW_3, "CNS");

    // Draw Numbers
    Tft.setFont(Terminal11x16);


    // Temp
    char temp[7];
    sprintf(temp, "%4.1f",data.Temperature);
    Tft.drawText(COLUMN_1, ROW_2+VAL_OFFSET, temp);

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
    sprintf(tts, "%02.0f",data.TTS);
    Tft.drawText(COLUMN_3, ROW_3+VAL_OFFSET, tts);

    // CNS
    char cns[7];
    sprintf(cns, "%02.0f",data.CNS);
    Tft.drawText(COLUMN_4, ROW_3+VAL_OFFSET, cns);
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
        char time[15];
        RealTime RTCReading = ReadRTC();
        sprintf(time,"%02d:%02d",RTCReading.Hour, RTCReading.Minute);
        Tft.drawText(150, textRow, time, COLOR_WHITE);
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
    Adc1.setGain(GAIN_TWOTHIRDS);
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "ADC2...");
    Adc2.begin();
    Adc2.setGain(GAIN_TWOTHIRDS);
    Tft.drawText(130, textRow, "OK", COLOR_GREEN);

    textRow += 15;
    Tft.drawText(10, textRow, "Depth Sensor...");
    if (!InitDepth()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        DepthSensor.setModel(MS5837::MS5837_30BA);
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
        char pressure[10];
        sprintf(pressure, "%.3fBar",fabs(SurfacePressure));
        Tft.drawText(150, textRow, pressure);
    }

    textRow += 15;
    Tft.drawText(10, textRow, "Deco Model...");
    Deco *testDeco = new Deco;
    float depth = 45;
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
    textRow += 15;
    Tft.drawText(10, textRow, "File System...");
    if (!LittleFS.begin()) {
        Tft.drawText(130, textRow, "ERR", COLOR_RED);
        pass = false;
    } else {
        Tft.drawText(130, textRow, "OK", COLOR_GREEN);
        char space[10];
        FSInfo fs_info{};
        LittleFS.info(fs_info);
        sprintf(space,"%zuk/%zuk", fs_info.usedBytes/1024,fs_info.totalBytes/1024);
        Tft.drawText(150, textRow, space, COLOR_WHITE);
    }

    textRow += 15;
    Tft.setFont(Terminal12x16);
    if (pass) {
        Tft.drawText(10, textRow, "ALL TESTS PASSED", COLOR_GREEN);
    } else {
        Tft.drawText(10, textRow, "FAILED TESTS: HALT", COLOR_RED);
    }
    return pass;
}

void ButtonOne()
{
    Serial.println("Button 1");
}

void ButtonTwo()
{
    Serial.println("Button 2");
}
