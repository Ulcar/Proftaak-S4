#include "SimpleCAN.h"
#include "SPI.h"


SimpleCAN::SimpleCAN(int canTimeout)
{
   Serial.println("Got here");
}

/*bool SimpleCAN::HandleTextMsg(CANMSG msg) //plakt nog niks aan elkaar
{
    Serial.print("Received msg: ID: ");
    Serial.print(msg.adrsValue);
    Serial.print(" MSG: ");
    //printByteArray(&msg.data[0], msg.dataLength);
    Serial.print("\n");
    return true;
} */




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

bool SimpleCAN::GetReceivedMessage(CANMSG *msg)
{
    return can.receiveCANMessage(msg, 10);
}

bool SimpleCAN::SendString(char buffer[], int size, uint8_t uniqueID, unsigned long adrsValue)
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


bool SimpleCAN::SendCANMSG(CANMSG msg)
{
   can.transmitCANMessage(msg, 1000);
}

bool SimpleCAN::SendInt(int val, unsigned long address)
{
   CANMSG msg = IntToCANMSG(val);
   msg.adrsValue = address;
   return SendCANMSG(msg);
} 

uint32_t SimpleCAN::CANMSGToUint32(CANMSG msg)
{
    return (msg.data[0] << 24) + (msg.data[1] << 16) + (msg.data[2] << 8) + msg.data[3];
}

CANMSG SimpleCAN::Uint32ToCANMSG(uint32_t val)
{
   CANMSG msg;
   msg.data[0] = val >> 24;
   msg.data[1] = val >> 16;
   msg.data[2] = val >> 8;
   msg.data[3] = val;
   return msg;
}

 CANMSG SimpleCAN::IntToCANMSG(int val)
 {
     CANMSG msg;
   msg.data[0] = val >> 8;
   msg.data[1] = val;
   return msg;
 }

 int SimpleCAN::CANMSGToInt(CANMSG msg)
 {
    return (msg.data[1] << 8) + (msg.data[0]);
 }

CANMSG SimpleCAN::Uint32ToCANMSGWithAddress(uint32_t val, unsigned long address)
{
   CANMSG msg = Uint32ToCANMSG(val);
   msg.adrsValue = address;
   return msg;
}

bool SimpleCAN::CANMSGToBool(CANMSG msg)
{
   //uh how are we gonna do this
   return msg.data[0];
}

 CANMSG SimpleCAN::BoolToCANMSG(bool val)
 {
    CANMSG msg;
    msg.data[0] = val;
    return msg;
 }