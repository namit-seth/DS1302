#ifndef DS1302_H
#define DS1302_H

#include "Arduino.h" // Needed for Arduino functions like digitalWrite()

class Time
{
  public:
    /* data */
    int sec;
    int min;
    int hour;
    int day;
    int month;
    int year;
    int DOW;
    Time(int sec, int min, int hour, int day, int month, int DOW, int year);
};

class DS1302 {
  public:
    DS1302(int RST, int DAT, int CLK); // Constructor
    ~DS1302(); // Destructor
    // Add return from Dump functions
    void CLK_Dump(int* Data);
    void RAM_Dump(int* Data);
    void Write_to_RAM(byte DATA, int LOCATION = 32);
    byte Read_from_RAM(int LOCATION = 32);
    Time Read_CLK();

    void Write_CLK(Time time);


  private:
    int _RST;
    int _DAT;
    int _CLK;
    int _RAM_LOC = 0;
    void WriteData(byte Word);
    void ReadData(int* Data, int Bytes);
    void WriteEnable();
};

#endif
