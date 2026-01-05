#include "DS1302.h"

DS1302 rtc(2, 3, 4);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Time t = rtc.Read_CLK();

  Serial.print("Time: ");
  Serial.print(t.hour); Serial.print(":");
  Serial.print(t.min);  Serial.print(":");
  Serial.println(t.sec);

  Serial.print("Date: ");
  Serial.print(t.day); Serial.print("/");
  Serial.print(t.month); Serial.print("/20");
  Serial.println(t.year);

  delay(1000);
}
