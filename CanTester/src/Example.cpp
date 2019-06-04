
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



void setup()
{
  Serial.begin(9600); 
  while(!Serial);
  Serial.println("Go...");
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  can = new SimpleCAN(100);
  can->Setup();
}


void loop()
{
  
  if(can->GetReceivedMessage(&msg))
  {
    if(msg.adrsValue == 11 || msg.adrsValue == 12)
    {
      Serial.print("Address: ");
      Serial.print(msg.adrsValue);
      Serial.print(" Int Value: ");
      Serial.println(can->CANMSGToInt(msg));
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







