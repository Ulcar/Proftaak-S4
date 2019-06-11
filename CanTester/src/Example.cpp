
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
const int inputLength = 100;
char data[inputLength];
byte senderID = 0;

void printByteArray(byte data[], int length) // prints 8-bit data in hex
{
  Serial.print("Hex Data: ");
  for (size_t i = 0; i < (size_t)length; i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void printCharArray(byte *arrayPtr, int size)
{
    for (int i = 0; i < size; i++)
    {
      Serial.write(arrayPtr, size);
    }
}

bool HandleTextMsg(CANMSG msg) //plakt nog niks aan elkaar
{
    Serial.print("Received msg: ID: ");
    Serial.print(msg.adrsValue);
    Serial.print(" MSG: ");
    // printCharArray(&msg.data[0], msg.dataLength);
    printByteArray(msg.data, msg.dataLength);
    Serial.print("\n");
    return true;
}

void setup()
{
  Serial.begin(9600); 
  while(!Serial);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  can = new SimpleCAN(100);
  can->Setup();
}

  bool messageToggle = false;
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
      byte* arr;
      int size;

      if(can->CANMSGToByteArray(msg, &arr, &size))
      {
        Serial.print("ID: ");
        Serial.print(msg.adrsValue);
        Serial.print(" Data: ");
        Serial.write((byte*)arr, size);
        // can->printByteArrayHex(arr, size);
        // if (can->PrintList(msg.data[0]) == -1)
        // {
        //   Serial.println("PrintList could not find list.");
        // }

        delete[] arr;
      }

    }
  }

 if(Serial.available() > 0)
 {
   
  char adr[10] = {}; 
  Serial.readBytesUntil(';', adr, 10);
  int size = Serial.readBytesUntil('\n', data, inputLength);

  // Serial.print("Sending data: ");
  Serial.write((byte *)data, size);

  if (!can->SendString(data, size, (int)senderID, atol(adr)))
  {
    Serial.println("Failed to send data.");
  }

  // can->SendLongByteMessage((byte *)data, size, atol(adr));
  
  // byte buffer[30] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E};
  // can->SendLongByteMessage(buffer, 30, atol(adr));
  senderID++;
  
 }
}







