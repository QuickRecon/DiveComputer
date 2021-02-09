// Include application, user and local libraries
#include "DiveComputer.h"

void ResetWatchdog() {
    ESP.wdtFeed();
}

void setup() {
    //WiFi.forceSleepBegin();
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
    Tft.setFont(Terminal6x8);
    Serial.begin(9600);
    bool pass = SelfTest();
    delay(2000); // Let some time for the user to look at the self test
    if (!pass) {
        Tft.setDisplay(false);
        ESP.restart();
    }

    //DecoActual.AddGas(0,1,0);
}

void loop() {
    PollButtons();

    UIData data = CollectData();
    UpdateUI(data);

}
