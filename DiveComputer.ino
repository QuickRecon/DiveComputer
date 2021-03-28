// Include application, user and local libraries
#include "DiveComputer.h"

void ResetWatchdog() {
    ESP.wdtFeed();
}

void setup() {
    // Disable wifi
    WiFi.forceSleepBegin();

    // Configure startup CS pins
    pinMode(TFT_CS, OUTPUT);
    pinMode(RTC_CS, OUTPUT);
    pinMode(TFT_LED, OUTPUT); // Override LED because PWM is software and we ain't got the time

    digitalWrite(TFT_CS, HIGH);
    digitalWrite(RTC_CS, HIGH);
    digitalWrite(TFT_LED, HIGH);

    Tft.begin();
    Tft.clear();
    Tft.setOrientation(3);
    Serial.begin(9600);

    // Register buttons

    Serial.println("Start self test");
    bool pass = SelfTest();
    delay(2000); // Let some Time for the user to look at the self test
    if (!pass) {
        Serial.println("FAIL self test!");
        //Tft.setDisplay(false);
        //ESP.restart();
        //MenuItem dummyItem;
        //TurnOffCallback(dummyItem); // Shutdown
    }
    Serial.println("Pass self test");
}

void loop() {
    PollButtons();
    UIData data = CollectData();
    UpdateUI(data);
    if (CurrUIState.Mode == DIVE) {
        UpdateDiveManager(data);
    } else if (CurrUIState.Mode == SURFACE &&
               data.Depth > SURFACE_DIVE_THRESHOLD) // Automatically start dive if deeper than 1m
    {
        MenuItem dummyItem;
        StartDiveCallback(dummyItem);
    }
}
