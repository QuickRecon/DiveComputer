// Include application, user and local libraries
#include "DiveComputer.h"

void ResetWatchdog()
{
    ESP.wdtFeed();
}

void setup() {
    Serial.begin(9600);
    Tft.begin();
    Tft.clear();
    Tft.setOrientation(1);
    Tft.setFont(Terminal6x8);

    bool pass = SelfTest();
    delay(2000); // Let some time for the user to look at the self test
    if (!pass) {
        ESP.restart();
    }

    DiveStartTime = ReadRTC();
}

void loop() {
    rtc.update();
    Serial.println(rtc.second());
}
