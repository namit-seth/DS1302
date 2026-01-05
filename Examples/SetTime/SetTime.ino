#include "DS1302.h"

DS1302 rtc(2, 3, 4);

void setup() {
  Serial.begin(9600);

  Time now(
    0,    // sec
    30,   // min
    14,   // hour (2 PM)
    5,    // day
    10,   // month
    7,    // DOW
    25    // year (2025)
  );

  rtc.Write_CLK(now);
  Serial.println("Time set successfully");
}

void loop() {
}
