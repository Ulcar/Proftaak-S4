#include <Arduino.h>
#include "Lcd.h"
#include <SPI.h>
#include <MCP2515.h>
#include <SimpleCAN.h>


#define MODE 1 //0=rx, 1=tx
int led2  =  7;
int led3  =  8;
int led13 = 13;
SimpleCAN* can;
CANMSG msg;
char data[100];

int speed = 50;
  int rpm = 1;
  int distance = 1;
  int fuel = 1;

Lcd lcd(20, 4, 6, 10);
SoftwareSerial iets(10,6);

void test();
void carDashBoardDisplay(int speed, int rpm, int distance, int fuel)
{
  char sp = (char)speed;
  char rp = (char)rpm;
  char dis = (char)distance;
  char fu = (char)fuel;
  lcd.ClearDisplay();
  lcd.SelectLine(1);
  lcd.WriteText("Speed:");
  lcd.MoveCursorRight(1);
  lcd.WriteInt(speed);
  lcd.SelectLine(2);
  lcd.WriteText("RPM:");
  lcd.MoveCursorRight(1);
  lcd.WriteInt(rpm);
  lcd.SelectLine(3);
  lcd.WriteText("KM:");
  lcd.MoveCursorRight(1);
  lcd.WriteInt(distance);
  lcd.SelectLine(4);
  lcd.WriteText("fuel:");
  lcd.MoveCursorRight(1);
  lcd.WriteInt(fuel);
}

void printByteArray(byte *arrayPtr, int size)
{
    for (int i = 0; i < size; i++)
    {
        Serial.print((char)arrayPtr[i]);
    }
}

bool HandleTextMsg(CANMSG msg) //plakt nog niks aan elkaar
{
    Serial.print("Received msg: ID: ");
    Serial.print(msg.adrsValue);
    Serial.print(" MSG: ");
    printByteArray(&msg.data[0], msg.dataLength);
    Serial.print("\n");
    return true;
}


void setup() {
  // put your setup code here, to run once:
  iets.begin(9600);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Go...");
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  can = new SimpleCAN(100);
  can->Setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  
    


     if(can->GetReceivedMessage(&msg))
  {
    if(msg.adrsValue == 11 || msg.adrsValue == 12 || msg.adrsValue == 13 || msg.adrsValue == 14)
    {
      Serial.print("Address: ");
      Serial.print(msg.adrsValue);
      Serial.print(" Int Value: ");
      int convertedValue = can->CANMSGToInt(msg);
      Serial.println(convertedValue);
      switch(msg.adrsValue)
      {
        case 11:
        distance = convertedValue;
        break;

        case 12:
        speed = convertedValue;
        break;
        case 13:
        rpm = convertedValue;
        break;
        case 14:
        fuel = convertedValue;
        break;
      }
      carDashBoardDisplay(speed, rpm, distance, fuel);
    }
    
    else
    {
      HandleTextMsg(msg);
    }

  }
  
 if(Serial.available() > 0)
 {
  char adr[10] = {}; 
  Serial.readBytesUntil(';', adr, 10);
  int size =  Serial.readBytesUntil('\n', data, 100);
  can->SendString(data, size, 0, atol(adr));
 }

}

void test()
{
  lcd.ClearDisplay();
  lcd.SelectLine(1);
  for (int i = 0; i < 5; i++) {
    lcd.WriteText("test123");
    lcd.MoveCursorRight(1);
    delay(500);
  }

}
