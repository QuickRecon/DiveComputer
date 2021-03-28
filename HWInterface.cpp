//
// Created by aren on 30/1/21.
//
#include "HWInterface.h"

// Accelerometer Params
Adafruit_LSM303_Accel_Unified Accel = Adafruit_LSM303_Accel_Unified(54321);

// Magnetometer Params
Adafruit_LSM303DLH_Mag_Unified Mag = Adafruit_LSM303DLH_Mag_Unified(12345);
CompassCalibrationCoefficients CompassCalibration{0, 0, 0};

// ADC1 Params
QR_ADS1115 Adc = QR_ADS1115(ADC_ADDRESS);

// Depth Sensor Params
MS5837 DepthSensor;

// Screen Params
TFT_22_ILI9225 Tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, 0, 255);

// IO Expander Params
//PCF8574 IOExpander(IO_ADDRESS);

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

bool InitIO() {
    // Set pin modes
    //IOExpander.pinMode(P0, OUTPUT);
    //IOExpander.pinMode(P1, OUTPUT);
    //IOExpander.pinMode(P2, OUTPUT);
    //IOExpander.pinMode(P3, INPUT);
    //IOExpander.pinMode(P4, INPUT);
    //IOExpander.pinMode(P5, INPUT);
    //IOExpander.pinMode(P6, INPUT);
    //IOExpander.pinMode(P7, INPUT);

    //IOExpander.digitalWrite(OUTPUT_ENABLE,LOW);
    //DisableReset();

    Wire.beginTransmission(IO_READ_ADDRESS);
    bool readOK = Wire.endTransmission() == 0;

    Wire.beginTransmission(IO_WRITE_ADDRESS);
    bool writeOK = Wire.endTransmission() == 0;

    return readOK && writeOK;
}

bool InitADC() {
    Adc.begin();
    Adc.setGain(GAIN_TWOTHIRDS);
    Adc.getLastConversionResults();

    Wire.beginTransmission(ADC_ADDRESS);
    return Wire.endTransmission() == 0;
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
    Wire.requestFrom(IO_READ_ADDRESS, (uint8_t) 1);
    byte IOByte = Wire.read();// Read a byte

    Serial.println(IOByte);

    bool button1 = (bit(BUTTON_1_PIN) & IOByte) == 0;
    bool button2 = (bit(BUTTON_2_PIN) & IOByte) == 0;

    Serial.print("Button 1: ");
    Serial.println(button1);
    Serial.print("Button 2: ");
    Serial.println(button2);

    if (button1 && button2) {
        // Implement 2 button combination?
    } else if (button1) {
        ButtonOne();
    } else if (button2) {
        ButtonTwo();
    }
}

double ReadHeading() {
    sensors_event_t event;
    Mag.getEvent(&event);

    double x = event.magnetic.x - CompassCalibration.x;
    double y = event.magnetic.y - CompassCalibration.y;
    double z = event.magnetic.z - CompassCalibration.z;


    double Pi = 3.14159;

    // Calculate the angle of the vector y,x
    double heading = (atan2(x, y) * 180) / Pi;

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

    screenData.Rate = Rate;

    CurrentSchedule = DecoActual.GetDecoSchedule(ResetWatchdog);

    if (CurrentSchedule.empty()) {
        screenData.NDL = DecoActual.GetNoDecoTime(ResetWatchdog);
        screenData.Stop = Deco::DecoStop();
    } else {
        screenData.Stop = CurrentSchedule[0];
        screenData.NDL = -1;
    }

    screenData.TTS = GetTTS(CurrentSchedule);

    return screenData;
}

void TurnOff() {
    // Turn off display
    Serial.println("Turning off");
    digitalWrite(TFT_LED, LOW);
    Tft.setDisplay(false);

    // Arm the reset circuit
    EnableReset();

    ESP.deepSleep(0); // Shut down
}

void EnableReset() {
    Wire.beginTransmission(IO_WRITE_ADDRESS); // Plus 1 to switch to write address
    Wire.write(LATCH_ENABLE_RESET_ENABLE);
    delay(10); // Delay a tad to allow the circuits to set up
    Wire.write(LATCH_DISABLE_RESET_ENABLE);
    Wire.endTransmission();
}

void DisableReset() {
    Wire.beginTransmission(IO_WRITE_ADDRESS);
    Wire.write(LATCH_ENABLE_RESET_DISABLE);
    delay(10); // Delay a tad to allow the circuits to set up
    Wire.write(LATCH_DISABLE_RESET_DISABLE);
    Wire.endTransmission();
}

double ReadBatteryVoltage() {
    int16_t batteryVal;
    batteryVal = analogRead(BATTERY_PIN);

    double dividerTop = 92.9;
    double dividerBottom = 97.7;

    return ((double) batteryVal) * (dividerTop + dividerBottom) / dividerBottom /
           1024; // 1/2 voltage divider on battery
}