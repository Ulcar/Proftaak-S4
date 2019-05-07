#include "SimpleCAN.h"
#include "SPI.h"

SimpleCAN::SimpleCAN(void (*fun_ptr_arr[])(CANMSG), int startValue, int size)
{
}
SimpleCAN::SimpleCAN()
{
   Serial.println("Got here");
}

bool SimpleCAN::HandleTextMsg(CANMSG msg) //plakt nog niks aan elkaar
{
    Serial.print("Received msg: ID: ");
    Serial.print(msg.adrsValue);
    Serial.print(" MSG: ");
    printByteArray(&msg.data[0], msg.dataLength);
    Serial.print("\n");
    return true;
}

uint32_t SimpleCAN::HandleUint32Msg(CANMSG msg) //big endian
{
    // https://stackoverflow.com/a/12240325
    uint32_t val = (msg.data[0] << 24) + (msg.data[1] << 16) + (msg.data[2] << 8) + msg.data[3];
    return val;
}

void SimpleCAN::MakeUint32Msg(CANMSG *msg, uint32_t val) //big endian
{
    // // https://stackoverflow.com/a/6502825
     msg->data[0] = val >> 24;
     msg->data[1] = val >> 16;
     msg->data[2] = val >> 8;
     msg->data[3] = val;
}
bool SimpleCAN::HandleReceivedMessage(CANMSG *msg)
{
    return can.receiveCANMessage(msg, 10);
}

void SimpleCAN::printByteArray(byte *arrayPtr, int size)
{
    for (int i = 0; i < size; i++)
    {
        Serial.print((char)arrayPtr[i]);
    }
}

void SimpleCAN::Setup()
{
      SPI.setClockDivider(SPI_CLOCK_DIV8);

  if (can.initCAN(CAN_BAUD_100K) == 0)
  {
    Serial.println("initCAN() failed");
    
    while (1);
  }

  if (can.setCANNormalMode(LOW) == 0) //normal mode non single shot
  {
    Serial.println("setCANNormalMode() failed");
    while (1);
  }

  Serial.print("Done setting up\n");
}

void SimpleCAN::SendCanString(char buffer[], int size, unsigned long adrsValue)
{
  int index = 0;
  Serial.print("size = ");
  Serial.println(size);
  Serial.print("adr = ");
  Serial.println(adrsValue);
  while(size > 0)
  {
     if(size > 8){
     if(BuildCanString(buffer + index, 8, adrsValue))
    {
       Serial.print("message sent:");
    }
       else
  {
     Serial.println("message send failed");
  }
    
     }
     else
     {
       if(BuildCanString(buffer + index, size, adrsValue))
    {
       Serial.println("message sent:");
    }
       else
  {
     Serial.println("message send failed");
  }
     }
     
 
    
    index += 8;
    size -= 8;
  }
}

bool SimpleCAN::BuildCanString(char buffer[], int size, unsigned long adrsValue)
{
  CANMSG messageToSend;
  messageToSend.adrsValue = adrsValue;
  messageToSend.isExtendedAdrs = false;
  messageToSend.rtr = false;
  messageToSend.dataLength = size;
  for(int i = 0; i < size; i++)
  {
    messageToSend.data[i] = buffer[i];
    Serial.print(buffer[i]);
  }
  Serial.println();
  return can.transmitCANMessage(messageToSend, 1000);

  
  
}