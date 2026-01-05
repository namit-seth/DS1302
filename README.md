# 📘 DS1302 Arduino Library

A lightweight, low-level Arduino library for the **DS1302 Real Time Clock (RTC)**.
This library directly implements the DS1302 3-wire protocol using GPIO bit-banging.

✔ No external dependencies
✔ Stable timing and pin control
✔ RAM read/write supported
✔ Clock burst read supported
✔ Suitable for learning, labs, and custom projects

---

## 📦 Library Structure

```
DS1302/
├── DS1302.h
├── DS1302.cpp
├── README.md
└── examples/
    ├── ReadTime/
    ├── SetTime/
    └── RAM_Test/
```

---

## 🔌 Hardware Connections

| DS1302 Pin | Arduino Pin (example) |
| ---------- | --------------------- |
| RST / CE   | D2                    |
| DAT / IO   | D3                    |
| CLK / SCLK | D4                    |
| VCC        | 5V                    |
| GND        | GND                   |
| VBAT       | 3V coin cell          |

> ⚠️ Use a **32.768 kHz crystal** with short leads
> ⚠️ VBAT is required to retain time when power is removed

---

## 🚀 Basic Usage

```cpp
#include "DS1302.h"

DS1302 rtc(2, 3, 4); // RST, DAT, CLK
```

---

## ⏰ Time Structure

```cpp
Time time(
  sec,    // 0–59
  min,    // 0–59
  hour,   // 0–23
  day,    // 1–31
  month,  // 1–12
  dow,    // 1–7 (Day of Week)
  year    // 0–99
);
```

---

## 📖 Public API

### Clock Functions

```cpp
Time Read_CLK();
void Write_CLK(Time time);
```

### RAM Functions

```cpp
void Write_to_RAM(byte data, int location);
byte Read_from_RAM(int location);
void RAM_Dump(int* buffer);
```

### Debug / Utility Functions

```cpp
void CLK_Dump(int* buffer);
```

---

## 🧪 Example 1: Read Current Time

**examples/ReadTime/ReadTime.ino**

```cpp
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
```

---

## 🧪 Example 2: Set Date & Time

**examples/SetTime/SetTime.ino**

```cpp
#include "DS1302.h"

DS1302 rtc(2, 3, 4);

void setup() {
  Serial.begin(9600);

  Time now(
    0,    // sec
    30,   // min
    14,   // hour
    5,    // day
    10,   // month
    7,    // DOW
    25    // year (2025)
  );

  rtc.Write_CLK(now);
  Serial.println("Time set successfully");
}

void loop() {}
```

---

## 🧪 Example 3: RAM Read / Write Test

**examples/RAM_Test/RAM_Test.ino**

```cpp
#include "DS1302.h"

DS1302 rtc(2, 3, 4);

void setup() {
  Serial.begin(9600);

  rtc.Write_to_RAM(0x42, 0);
  rtc.Write_to_RAM(0x99, 1);

  byte a = rtc.Read_from_RAM(0);
  byte b = rtc.Read_from_RAM(1);

  Serial.print("RAM[0] = 0x");
  Serial.println(a, HEX);

  Serial.print("RAM[1] = 0x");
  Serial.println(b, HEX);
}

void loop() {}
```

---

## 🧠 Notes & Tips

* DS1302 communicates **LSB first**
* CLK must be **LOW before RST goes HIGH**
* RAM size is **31 bytes** (addresses 0–30)
* Year is stored as **00–99**

---

## 🛠 Tested With

* Arduino Uno / Nano
* DS1302 modules (including clones)
* 5V logic

---

## 📜 License

MIT License
Free to use for learning, labs, and projects.
