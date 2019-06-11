#ifndef Lcd_h
#define Lcd_h

#include "Arduino.h"
#include <SoftwareSerial.h>


class Lcd
{
  public:
    Lcd(int width, int lines, int txPin, int rxPin);
    ~Lcd();
    void ClearDisplay();
    void MoveCursorRight(int spaces);
    void MoveCursorLeft(int spaces);
    //void ScrollRight(int spaces);
    //void ScrollLeft(int spaces);
    void DisplayOn_OR_HideCurser();
    void DisplayOff();
    //void UnderLineCurserOn();
    //void BlinkingCurserOn();
    void ToggleSplashScreen();
    void WriteText(const char* input);
    void WriteInt(int input);
    void SelectLine(int line);
  private:
    //int _widht;
    //int _height;
    //int _txPin;
    //int _rxPin;
    int _currentLine;
    int _curserPosition;
    int _splashScreen;
    SoftwareSerial *lcdSerial;
};

#endif
