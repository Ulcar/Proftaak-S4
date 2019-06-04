
#include <Arduino.h>
#include <SPI.h>
#include <MCP2515.h>
#include <SimpleCAN.h>
#include "CarDataSimulator.h"


#define MODE 1 //0=rx, 1=tx
int led2  =  7;
int led3  =  8;
int led13 = 13;
SimpleCAN* can;
CANMSG msg;
CarDataSimulator carData(1);
CarData data;
char charData[100];



int functionCounter = 0;
bool shouldSend = false;


void setupTimer1()
{
   cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 1 Hz increments
OCR1A = 62499; // = 16000000 / (256 * 1) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 256 prescaler
TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei(); // allow interrupts

}

void setupTimer2()
{

}

void setup()
{
  Serial.begin(9600); 
  while(!Serial);
  Serial.println("Go...");
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  can = new SimpleCAN(10);
  can->Setup();
  carData.SetTargetSpeed(120);
  setupTimer1();
  sei();
}

void SendSpeed()
{
  can->SendInt(data.speed, 12);
}
void SendDistance()
{
   can->SendInt(data.m, 11);
}
void SendFuel()
{

}
void SendRPM(){}

void loop()
{
 Serial.println(data.m);
  
 if(Serial.available() > 0)
 {
 /* char adr[10] = {}; 
  Serial.readBytesUntil(';', adr, 10);
  int size =  Serial.readBytesUntil('\n', charData, 100);
  can->SendString(charData, size, 0, atol(adr));
  */

 char speed[10] = {}; 
  Serial.readBytesUntil('\n', speed, 8);
 carData.SetTargetSpeed(atol(speed));
 }

//change this to function pointer array?
if(shouldSend){
  switch (functionCounter)
  {
    case 0:
    SendSpeed();
    break;

    case 1:
    SendDistance();
    break;

    case 2:
    SendFuel();
    break;

    case 3:
    SendRPM();
    break;
  }
  shouldSend = false;
}


}



//careful here, timer interrupt takes priority over can sending
ISR(TIMER1_COMPA_vect)
{

carData.UpdateValues();
data = carData.GetData();


functionCounter++;
functionCounter = functionCounter % 4;
shouldSend = true;

}








