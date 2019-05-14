
#include <Arduino.h>
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


void Test(CANMSG msg)
{
  Serial.println("Ok this is epic");
}

void Test2(CANMSG msg)
{
  Serial.println("In second function");
}

void setup()
{
  Serial.begin(9600); 
  while(!Serial);
  Serial.println("Go...");
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  can = new SimpleCAN();
  can->Setup();
}


void loop()
{
  can->HandleReceivedMessage(&msg);
  
 if(Serial.available() > 0)
 {
  char adr[10] = {}; 
  Serial.readBytesUntil(';', adr, 10);
  int size =  Serial.readBytesUntil('\n', data, 100);
  can->SendCanString(data, size, atol(adr));
 }

}





