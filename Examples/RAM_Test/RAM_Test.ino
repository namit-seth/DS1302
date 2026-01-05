#include "DS1302.h"

DS1302 rtc(2, 3, 4);

void setup() {
  Serial.begin(9600);

  rtc.Write_to_RAM(0x42, 0);   // Write value to RAM[0]
  rtc.Write_to_RAM(0x99, 1);   // Write value to RAM[1]

  byte a = rtc.Read_from_RAM(0);
  byte b = rtc.Read_from_RAM(1);

  Serial.print("RAM[0] = 0x");
  Serial.println(a, HEX);

  Serial.print("RAM[1] = 0x");
  Serial.println(b, HEX);
}

void loop() {
}
