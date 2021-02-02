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
    Tft.setBacklightBrightness(TFT_BRIGHTNESS);
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
    PollButtons();

    UIData data = CollectData();
    ShowDiveScreen(data);

}
