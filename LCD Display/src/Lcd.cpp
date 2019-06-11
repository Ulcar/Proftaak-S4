#include "Arduino.h"
#include "Lcd.h"


Lcd::Lcd(int width, int lines, int txPin, int rxPin)
{
  lcdSerial = NULL;
  lcdSerial = new SoftwareSerial(rxPin,txPin);
  lcdSerial->begin(9600);
  if (width == 20) {
    lcdSerial->write(0x7C); //command flag
    lcdSerial->write(0x03);     //20 char wide
  }
  else if (width == 16) {
    lcdSerial->write(0x7C); //command flag
    lcdSerial->write(0x04);     //16 char wide
  }
  if (lines == 4) {
    lcdSerial->write(0x7C); //command flag
    lcdSerial->write(0x05);     //4 lines
  }
  else if (lines == 2) {
    lcdSerial->write(0x7C); //command flag
    lcdSerial->write(0x06);     //2 lines
  }
}

Lcd::~Lcd()
{
  if(lcdSerial != NULL) free(lcdSerial);
}

void Lcd::ClearDisplay()
{
  lcdSerial->write(0xFE);
  lcdSerial->write(0x01);
}

void Lcd::MoveCursorRight(int spaces)
{
  for (int i = 0; i < spaces; i++) {
    lcdSerial->write(0xFE); //command flag
    lcdSerial->write(0x14); // 0x14
  }
}

void Lcd::MoveCursorLeft(int spaces)
{
  for (int i = 0; i < spaces; i++) {
    lcdSerial->write(0xFE);
    lcdSerial->write(0x10);
  }
}

void Lcd::DisplayOn_OR_HideCurser()
{
  lcdSerial->write(0xFE);
  lcdSerial->write(0x0C);
}

void Lcd::DisplayOff()
{
  lcdSerial->write(0xFE);
  lcdSerial->write(0x08);
}

void Lcd::ToggleSplashScreen()
{
  lcdSerial->write(0x7C); //command flag = 124 dec
  lcdSerial->write(0x09); // 0x09
}

void Lcd::WriteText(const char *input)
{
  lcdSerial->write(input);
}

void Lcd::WriteInt(int input)
{
  char snum[5];
  itoa(input, snum, 10);
  WriteText(snum);
}

void Lcd::SelectLine(int line)
{
  switch (line) {
    case 1:
    lcdSerial->write(0xFE);
    lcdSerial->write(128);
    break;
    case 2:
    lcdSerial->write(0xFE);
    lcdSerial->write(192);
    break;
    case 3:
    lcdSerial->write(0xFE);
    lcdSerial->write(148);
    break;
    case 4:
    lcdSerial->write(0xFE);
    lcdSerial->write(212);
    break;
    default:
    break;
  }

}
