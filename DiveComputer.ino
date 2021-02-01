// Include application, user and local libraries
#include "DiveComputer.h"

void ResetWatchdog()
{
    ESP.wdtFeed();
}

void setup() {
    Serial.begin(9600);
    pinMode(TFT_CS, OUTPUT);
    pinMode(RTC_CS, OUTPUT);

    digitalWrite(TFT_CS, HIGH);
    digitalWrite(RTC_CS, HIGH);

    Tft.begin();
    Tft.setBacklightBrightness(TFT_BRIGHTNESS)
    Tft.clear();
    Tft.setOrientation(1);
    Tft.setFont(Terminal6x8);

    bool pass = SelfTest();
    delay(2000); // Let some time for the user to look at the self test
    if (!pass) {
        Tft.setDisplay(false);
        ESP.restart();
    }
    DiveStartTime = ReadRTC();
}

void loop() {
    UIData data = CollectData();
    ShowDiveScreen(data);

    int16_t adc_val0, adc_val1, adc_val2, adc_val3;
    adc_val0 = Adc1.readADC_SingleEnded(0);
    adc_val1 = Adc1.readADC_SingleEnded(1);
    adc_val2 = Adc1.readADC_SingleEnded(2);
    adc_val3 = Adc1.readADC_SingleEnded(3);
    #ifdef EnableSerial
    //Serial.print("AIN0: "); Serial.println(adc_val0);
    //Serial.print("AIN1: "); Serial.println(adc_val1);
    //Serial.print("AIN2: "); Serial.println(adc_val2);
    //Serial.print("AIN3: "); Serial.println(adc_val3);
    //Serial.println(" ");
    #endif
}
