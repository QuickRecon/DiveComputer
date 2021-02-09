//
// Created by aren on 30/1/21.
//
#include "HWInterface.h"

double LastButton1Val = 1000;
double LastButton2Val = 1000;

// Accelerometer Params
Adafruit_LSM303_Accel_Unified Accel = Adafruit_LSM303_Accel_Unified(54321);

// Magnetometer Params
Adafruit_LSM303DLH_Mag_Unified Mag = Adafruit_LSM303DLH_Mag_Unified(12345);

// ADC1 Params
QR_ADS1115 Adc1 = QR_ADS1115(0x49);

// ADC2 Params
QR_ADS1115 Adc2 = QR_ADS1115(0x48);

// Depth Sensor Params
MS5837 DepthSensor;

TFT_22_ILI9225 Tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, 0, 255);
//TFT_22_ILI9225 Tft  = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED, 255);


bool InitRTC() {
    rtc.begin(RTC_CS);
    if (SET_CLOCK) {
        rtc.autoTime();
    }
    bool status = ReadRTC().Second <= 60 && ReadRTC().Minute <= 60 && ReadRTC().Hour <= 24;

    return status; // Sanity check to ensure RTC is working
}

bool InitDepth() {
    bool status = DepthSensor.init();
    SurfacePressure = ReadDepthSensor().Pressure / 1000.0;
    return status;
}

DepthSensorData ReadDepthSensor() {
    DepthSensor.read();
    DepthSensorData data{};
    data.Pressure = DepthSensor.pressure();
    data.Temperature = DepthSensor.temperature();
    return data;
}

RealTime ReadRTC() {
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

void PollButtons() {
    int16_t button1Val, button2Val;
    button1Val = Adc1.readADC_SingleEnded(BUTTON_1_CHANNEL); // Button 1 on left
    button2Val = Adc1.readADC_SingleEnded(BUTTON_2_CHANNEL); // Button 2 on right

    //Serial.print("Button 1: ");
    //Serial.println(button1Val);
    //Serial.print("Button 2: ");
    //Serial.println(button2Val);

    bool button1 = button1Val < LastButton1Val - BUTTON_THRESHOLD;
    bool button2 = button2Val < LastButton2Val - BUTTON_THRESHOLD;

    LastButton1Val = button1Val;
    LastButton2Val = button2Val;

    if (button1 && button2) {
        if (button1Val < button2Val) {
            ButtonOne();
        } else {
            ButtonTwo();
        }
    } else if (button1) {
        ButtonOne();
    } else if (button2) {
        ButtonTwo();
    }
}

double ReadHeading() {
    sensors_event_t event;
    Mag.getEvent(&event);

    double Pi = 3.14159;

    // Calculate the angle of the vector y,x
    double heading = (atan2(event.magnetic.z, event.magnetic.x) * 180) / Pi;

    // Normalize to 0-360
    if (heading < 0) {
        heading = 360 + heading;
    }

    return heading;
}

UIData CollectData() {
    UIData screenData{};

    DepthSensorData depthData = ReadDepthSensor();
    screenData.AmbientPressure = depthData.Pressure / 1000.0;
    screenData.Depth = BarToMeter(depthData.Pressure / 1000.0);
    screenData.Temperature = depthData.Temperature;

    screenData.Heading = ReadHeading();

    screenData.Time = ReadRTC();

    screenData.Gas = GetCurrGas();

    screenData.AverageDepth = AverageDepth;

    screenData.CNS = CNS;

    screenData.DiveTime = TimeDiff(ReadRTC(), DiveStartTime);

    screenData.PPO2 = screenData.Gas.FrO2 * screenData.AmbientPressure;

    screenData.Rate = 0;

    CurrentSchedule = GetDecoSchedule();

    if (GetDecoSchedule().empty()) {
        screenData.NDL = DecoActual.GetNoDecoTime();
        screenData.Stop = Deco::DecoStop();
    } else {
        screenData.Stop = CurrentSchedule[0];
        screenData.NDL = -1;
    }

    screenData.TTS = GetTTS(CurrentSchedule);

    return screenData;
}

void TurnOff(){
    //Serial.println("Turning off");
    digitalWrite(TFT_LED, LOW);
    Tft.setDisplay(false);
    Adc1.startComparator_SingleEnded(BUTTON_1_CHANNEL, LastButton1Val+100);


    wifi_station_disconnect();
    wifi_set_opmode_current(NULL_MODE);
    wifi_fpm_set_sleep_type(
            LIGHT_SLEEP_T); // set sleep type, the above    posters wifi_set_sleep_type() didnt seem to work for me although it did let me compile and upload with no errors
    wifi_fpm_open(); // Enables force sleep
    gpio_pin_wakeup_enable(GPIO_ID_PIN(PWR_UP_PIN),
                           GPIO_PIN_INTR_LOLEVEL); // GPIO_ID_PIN(2) corresponds to GPIO2 on ESP8266-01 , GPIO_PIN_INTR_LOLEVEL for a logic low, can also do other interrupts, see gpio.h above
    wifi_fpm_do_sleep(0xFFFFFFF); // Sleep for longest possible time
    delay(500);
    // Be Asleep
    ESP.restart(); //On Wake restart
}

double ReadBatteryVoltage() {
    int16_t batteryVal;
    batteryVal = Adc1.readADC_SingleEnded(BATTERY_CHANNEL);

    return ((double) batteryVal) * ADC_1_V_PER_BIT;
}