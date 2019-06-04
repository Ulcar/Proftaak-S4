#include "SimpleCAN.h"
#include "SPI.h"


SimpleCAN::SimpleCAN(void (*fun_ptr_arr[])(CANMSG), int startValue, int size)
{
}

SimpleCAN::SimpleCAN(int canTimeout)
: canTimeout(canTimeout)
{
   Serial.println("SimpleCAN library started");
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

bool SimpleCAN::SendCANMSG(CANMSG msg)
{
   return can.transmitCANMessage(msg, 1000);
}

bool SimpleCAN::SendByteMessage(byte buffer[], int size, unsigned long address)
{
   if (size > 8)
   {
      Serial.println("Failed to send message, size too big, maximum is 8 bytes.");
      return false;
   }

   CANMSG msg;
   msg.adrsValue = address;
   msg.dataLength = size;
   if (buffer != NULL && size != 0)
   {
      for (int i = 0; i < size; i++)
      {
         msg.data[i] = buffer[i];
      }
   }
   else if ((buffer == NULL && size != 0) || (buffer != NULL && size == 0))
   {
      Serial.println("Failed to send message, bad parameters.");
      return false;
   }
   return can.transmitCANMessage(msg, 1000);
}

bool SimpleCAN::SendCharMsg(char buffer[], int size, unsigned long address)
{
   CANMSG messageToSend;
   messageToSend.adrsValue = address;
   messageToSend.isExtendedAdrs = false;
   messageToSend.rtr = false;
   messageToSend.dataLength = size;
   for (int i = 0; i < size; i++)
   {
      messageToSend.data[i] = buffer[i];
      Serial.print(buffer[i]);
   }
   Serial.println();
   return can.transmitCANMessage(messageToSend, 1000);
}

bool SimpleCAN::GetReceivedMessage(CANMSG *msg)
{
   return can.receiveCANMessage(msg, 10);
}

uint32_t SimpleCAN::CANMSGToUint32(CANMSG msg) //big endian
{
    // https://stackoverflow.com/a/12240325
    uint32_t val = (msg.data[0] << 24) + (msg.data[1] << 16) + (msg.data[2] << 8) + msg.data[3];
    return val;
}

CANMSG SimpleCAN::Uint32ToCANMSG(uint32_t val) //big endian
{
    // // https://stackoverflow.com/a/6502825
   CANMSG msg;

   msg.data[0] = val >> 24;
   msg.data[1] = val >> 16;
   msg.data[2] = val >> 8;
   msg.data[3] = val;

   return msg;
}

bool SimpleCAN::SendString(char buffer[], int size, uint8_t uniqueID, unsigned long adrsValue)
{
   // TODO veranderen naar de nieuwe Extended String functie

  int index = 0;
  Serial.print("size = ");
  Serial.println(size);
  Serial.print("adr = ");
  Serial.println(adrsValue);
  while(size > 0)
  {
     if(size > 8){
     if(SendCharMsg(buffer + index, 8, adrsValue))
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
       if(SendCharMsg(buffer + index, size, adrsValue))
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

bool SimpleCAN::CANMSGToString(CANMSG msg, char *str, int *size)
{
   // TODO: verbeter naar de nieuwe Extended String versie

   // Gooi de berichten in een linked list en sorteer ze
   // Timeout tussen berichten, wanneer er een bericht out of sync binnenkomt weet je bij welke extended string hij hoort


   Serial.print("Received msg: ID: ");
   Serial.print(msg.adrsValue);
   Serial.print(" MSG: ");
   printByteArray(&msg.data[0], msg.dataLength);
   Serial.print("\n");
   return true;
}


void SimpleCAN::printByteArray(byte *arrayPtr, int size)
{
   for (int i = 0; i < size; i++)
   {
      Serial.print((char)arrayPtr[i]);
   }
}
