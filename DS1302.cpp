#include "DS1302.h"


const byte CMD_WORD_CLK_BURST = 0xBF; // 0b 1011 1111  clk burst read command word
const int CLK_Bytes = 9; // Number of Bytes in CLK registers
const byte CMD_WORD_RAM_BURST = 0XFF; // 0b 1111 1111 RAM burst read command word
const int RAM_Bytes = 31; // Number of Bytes in RAM
const int WE = 1 << 7; // Write Enable
const int RAM = 1 << 6; // CMD word RAM indicator
const int CLK = 0 << 6; // CMD word CLK indicator
const int WRITE = 0; // CMD word WRITE indicator
const int READ = 1; // CMD word READ indicator

int BCDToDec(const int bcd) {
  return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

int DecToBCD(const int dec) {
  const int tens = dec / 10;
  const int ones = dec % 10;
  return (tens << 4) | ones;
}

DS1302::DS1302(int RST, int DAT, int CLK){
    _RST = RST;
    _DAT = DAT;
    _CLK = CLK;

    pinMode(_CLK, OUTPUT);
    pinMode(_RST, OUTPUT);
    pinMode(_DAT, OUTPUT);

    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, HIGH);
    delayMicroseconds(1);
}

void DS1302::WriteEnable(){
  digitalWrite(_CLK, LOW);
  delayMicroseconds(1);
  digitalWrite(_RST, HIGH);
  delayMicroseconds(1);

  WriteData(0x8E);
  WriteData(0x00);

  digitalWrite(_RST, LOW);
  delayMicroseconds(1);

}

void DS1302::WriteData(byte Word){
  pinMode(_DAT, OUTPUT);
  digitalWrite(_CLK, LOW);
  delayMicroseconds(1);
  byte mask = 0b00000001;
  for(int i = 0; i < 8; i++){
    int bit = (mask & Word)? 1:0;
    digitalWrite(_DAT,bit);
    mask = mask << 1;
    digitalWrite(_CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(_CLK, LOW);
    delayMicroseconds(1);
  }
}

void DS1302::ReadData(int* Data, int Bytes){
  pinMode(_DAT, INPUT_PULLUP);
  for(int i = 0; i < Bytes; i++){
    byte data = 0;
    digitalWrite(_CLK,LOW);
    delayMicroseconds(1);
    for(int j = 0; j < 8; j++){
      int bit = digitalRead(_DAT);
      data = data >> 1;
      if(bit) data |= 0x80;
      digitalWrite(_CLK, HIGH);
      delayMicroseconds(1);
      if(j < 7){
        digitalWrite(_CLK, LOW);
        delayMicroseconds(1);
      }
    }
    Data[i] = data;
  }
}

void DS1302::CLK_Dump(int* Data){
    if(!Data) return;
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, HIGH);
    delayMicroseconds(1);
    WriteData(CMD_WORD_CLK_BURST);
    ReadData(Data, CLK_Bytes);
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, LOW);
    delayMicroseconds(1);
}

void DS1302::RAM_Dump(int* Data){
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, HIGH);
    delayMicroseconds(1);
    WriteData(CMD_WORD_RAM_BURST);
    ReadData(Data, RAM_Bytes);
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, LOW);
    delayMicroseconds(1);
}

void DS1302::Write_to_RAM(byte DATA, int LOCATION){
  WriteEnable();
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, HIGH);
    delayMicroseconds(1);
  if((LOCATION > 31) || (LOCATION < 0)){
    LOCATION = _RAM_LOC;
    if(_RAM_LOC != 31){
    _RAM_LOC++;
    }
  }

  byte WORD = WE | RAM | (LOCATION << 1) | WRITE;
  WriteData(WORD);
  WriteData(DATA);
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, LOW);
    delayMicroseconds(1);
}

byte DS1302::Read_from_RAM(int LOCATION){
    digitalWrite(_CLK, LOW);
    digitalWrite(_RST, HIGH);
    delayMicroseconds(1);
  if((LOCATION > 31) || (LOCATION < 0)){
    LOCATION = _RAM_LOC;
    if(_RAM_LOC != 31){
    _RAM_LOC++;
    }
  }
  int address = LOCATION << 1;

  byte WORD = WE | RAM | (LOCATION << 1) | READ;
  WriteData(WORD);
  int Data[1];
  ReadData(Data, 1);
  digitalWrite(_CLK, LOW);
  digitalWrite(_RST, LOW);
  delayMicroseconds(1);
  return (byte)Data[0];
}

Time DS1302::Read_CLK(){
  int CLK_data[9];
  CLK_Dump(CLK_data);
  Time time(BCDToDec(CLK_data[0]),
            BCDToDec(CLK_data[1]),
            BCDToDec(CLK_data[2]),
            BCDToDec(CLK_data[3]),
            BCDToDec(CLK_data[4]),
            BCDToDec(CLK_data[5]),
            BCDToDec(CLK_data[6]));
  return time;
}

void DS1302::Write_CLK(Time time){
  WriteEnable();
  digitalWrite(_CLK, LOW);
  digitalWrite(_RST, HIGH);
  delayMicroseconds(1);
  int address = 31;
  byte WORD = WE + CLK + address + WRITE;
  WriteData(WORD);
  WriteData(DecToBCD(time.sec));
  WriteData(DecToBCD(time.min));
  WriteData(DecToBCD(time.hour));
  WriteData(DecToBCD(time.day));
  WriteData(DecToBCD(time.month));
  WriteData(DecToBCD(time.DOW));
  WriteData(DecToBCD(time.year));
  digitalWrite(_RST, LOW);
  delayMicroseconds(1);
}

Time::Time(int sec, int min, int hour, int day, int month, int DOW, int year){
    this->sec = sec;
    this->min = min;
    this->hour = hour;
    this->day = day;
    this->month = month;
    this->year = year;
    this->DOW = DOW;
}