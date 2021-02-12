// Include application, user and local libraries
#include "DiveComputer.h"

void ResetWatchdog() {
    ESP.wdtFeed();
}

void setup() {
    WiFi.forceSleepBegin();
    pinMode(TFT_CS, OUTPUT);
    pinMode(RTC_CS, OUTPUT);
    pinMode(TFT_LED, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(PWR_UP_PIN, INPUT);

    digitalWrite(TFT_CS, HIGH);
    digitalWrite(RTC_CS, HIGH);
    digitalWrite(TFT_LED, HIGH);

    Tft.begin();
    Tft.clear();
    Tft.setOrientation(1);
    Serial.begin(9600);
    bool pass = SelfTest();
    delay(2000); // Let some Time for the user to look at the self test
    if (!pass) {
        Tft.setDisplay(false);
        ESP.restart();
    }
}

void loop() {
    PollButtons();

    UIData data = CollectData();
    UpdateUI(data);
    if (CurrUIState.Mode == DIVE) {
        UpdateDiveManager(data);
    } else if (CurrUIState.Mode == SURFACE && data.Depth > 1.0) // Automatically start dive if deeper than 1m
    {
        MenuItem dummyItem;
        StartDiveCallback(dummyItem);
    }
}
