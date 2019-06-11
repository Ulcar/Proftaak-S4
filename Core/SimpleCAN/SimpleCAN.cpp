// to do
/*
List ID is nu UniqueSenderID, dit kan beter het address waaop het bericht binnenkomt op de CANbus zijn. En daaronder kunnen dan beter verschillende lijsten met datalijsten komen. Dus eigenlijk een drie dimensionale linked list
Dus dan is het:
Lists met CANbus addressen
   Lists met MessageID
      Lists met Data

 */

#include "SimpleCAN.h"
#include "SPI.h"

SimpleCAN::SimpleCAN(int canTimeout)
    : canTimeout(canTimeout)
{
   lists = NULL;
   extendedMessageTimeout = 1000;
   Serial.println("SimpleCAN instance created.");
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

  Serial.print("SimpleCAN Setup done\n");
}

bool SimpleCAN::GetReceivedMessage(CANMSG *msg)
{
    return can.receiveCANMessage(msg, 10);
}

bool SimpleCAN::GetReceivedMessageWithAddress(CANMSG *msg, unsigned long *address)
{
   if(GetReceivedMessage(msg))
   {
      *address = msg->adrsValue;
      return true;
   }
   else
   {
      return false;
   }
   
}

bool SimpleCAN::SendCharMsg(char buffer[], int size,  unsigned long adrsValue)
{
   return SendByteMessage((byte*)buffer, size, adrsValue);
}

bool SimpleCAN::SendByteMessage(byte buffer[], int size, unsigned long adrsValue) //kapoet???
{
   // Serial.print("byte size: ");
   // Serial.println(size);

   // Serial.print("byte Sending data: ");
   // for (size_t i = 0; i < size; i++)
   // {
   //    Serial.print(buffer[i], HEX);
   //    Serial.print(' ');
   // }
   // Serial.println();

   CANMSG messageToSend;
   messageToSend.adrsValue = adrsValue;
   messageToSend.isExtendedAdrs = false;
   messageToSend.rtr = false;
   messageToSend.dataLength = size;
   for (int i = 0; i < size; i++)
   {
      messageToSend.data[i] = buffer[i];
      // Serial.print(buffer[i]);
   }
   // Serial.println();
   bool result = SendCANMSG(messageToSend);

   if(!result)
   {
      Serial.println("CAN doet raar");
   }

   return result;
}

bool SimpleCAN::SendPartOfLongByteMessage(byte buffer[], int size, byte senderID, byte messageNumber, unsigned long adrsValue)
{
   if (size > 6 || size < 1)
   {
      Serial.println("Invalid size in SendPartOfLongByteMessage");
      return false;
   }
   if (buffer == NULL)
   {
      Serial.println("Invalid buffer in SendPartOfLongByteMessage");
      return false;
   }
   if(senderID < 0 || senderID > 255)
   {
      Serial.println("Invalid senderID in SendPartOfLongByteMessage");
      return false;
   }
   
   // Serial.print("size: ");
   // Serial.println(size);

   // Serial.print("senderID: ");
   // Serial.println(senderID);

   // Serial.print("MessageNumber: ");
   // Serial.println(messageNumber);

   // Serial.print("Sending data: ");
   // for (size_t i = 0; i < (size_t)size; i++)
   // {
   //    Serial.print(buffer[i], HEX);
   //    Serial.print(' ');
   // }
   // Serial.println();

   byte arr[size + 2];
   arr[0] = senderID;
   arr[1] = messageNumber;
   for (size_t i = 0; i < (size_t)size; i++)
   {
      arr[i+2] = buffer[i];
   }

   // Serial.print("Arr data: ");
   // for (size_t i = 0; i < size+2; i++)
   // {
   //    Serial.print(arr[i], HEX);
   //    Serial.print(' ');
   // }
   // Serial.println();

   if (!SendByteMessage(arr, size+2, adrsValue))
   {
      return false;
   }
   return true;
}

bool SimpleCAN::SendLongByteMessage(byte buffer[], int size, unsigned long adrsValue, int senderID)
{
   if (size > (255 * 6))
   {
      Serial.println("size too big");
      return false;
   }
   if (size < 1)
   {
      Serial.println("Can't send empty messages");
      return false;
   }
   if (buffer == NULL)
   {
      return false;
   }
   
   bool spareBytes = size % 6 > 0;

   // Send first message with amount of total messages
   byte NumberOfMessages = size / 6 + spareBytes;
   SendPartOfLongByteMessage(&NumberOfMessages, 1, senderID, 0, adrsValue);

   // Serial.print("NumberOfMessages: ");
   // Serial.println(NumberOfMessages);

   byte messageNumber = 0;
   for (; messageNumber < (size / 6); messageNumber++)
   {
      if(!SendPartOfLongByteMessage(buffer + (messageNumber * 6), 6, senderID, messageNumber+1, adrsValue))
      {
         Serial.println("Could not send part of long message");
         return false;
      }
   }

   if (spareBytes)
   {
      int bytesLeft = size - (messageNumber * 6);

      if (!SendPartOfLongByteMessage(buffer + (messageNumber * 6), bytesLeft, senderID, messageNumber + 1, adrsValue))
      {
         Serial.println("Could not send spare bytes of long message");
         return false;
      }
   }
   return true;
}

bool SimpleCAN::SendString(char buffer[], int size, int senderID, unsigned long adrsValue)
{
   return SendLongByteMessage((byte*)buffer, size, adrsValue, senderID);
}

bool SimpleCAN::SendCANMSG(CANMSG msg)
{
   // Serial.print("Sending CANMSG data: ");
   // printByteArrayHex(msg.data, msg.dataLength);
   return can.transmitCANMessage(msg, canTimeout);
}

bool SimpleCAN::SendInt(int val, unsigned long address)
{
   CANMSG msg = IntToCANMSG(val);
   msg.adrsValue = address;
   msg.isExtendedAdrs = false;
   msg.rtr = false;
   msg.dataLength = 2;
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
   return (msg.data[0] << 8) + (msg.data[1]);
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

bool SimpleCAN::CANMSGToByteArray(CANMSG msg, byte** arr, int *size)
{
   int listID = msg.data[0];
   MSGDATALIST* list = FindList(listID);
   bool listExists = list != NULL;
   
   if (msg.data[1] == 0) // Makes a new list when the length message is received
   {
      // Serial.print("Expected Listsize: ");
      // Serial.println(msg.data[2]);

      if (listExists) // edit list size if it already exists
      {
         list->expectedListSize = msg.data[2];
      }
      if(CreateNewList(listID, msg.data[2]) != 0)
      {
         Serial.println("Failed to create a list to store extendedMessage data with length message.");
      }
      else
      {
         // Serial.println("List has been made.");         
      }
      
      return false;
   }
   else if (!listExists) // also makes a new list if length message happens to not be the first to be received
   {
      if (CreateNewList(listID, 255) != 0)
      {
         Serial.println("Failed to create a list to store extendedMessage data for unaligned message.");
      }
   }

   MSGDATA *newData = new MSGDATA;
   newData->data = new byte[msg.dataLength];
   
   newData->dataID = msg.data[1];
   newData->size = msg.dataLength-2;
   newData->next = NULL;
   for (size_t i = 0; i < (size_t)msg.dataLength; i++)
   {
      newData->data[i] = msg.data[i+2];
   }

   int result = AddMsgData(listID, newData, arr, size);
   switch (result)
   {
   case 1:
      // Serial.println("Full message has been received.");
      return true;
      break;
   case 0:
      // Serial.print("Data has been added to list: ");
      // Serial.println(listID);
      // PrintList(listID);
      break;
   case -1:
      Serial.println("This list does not exist yet.");
      break;
   case -2:
      Serial.println("NewData is NULL or Invalid");
      break;
   case -3:
      Serial.println("Data with those IDs already exists");
      break;

   default:
      break;
   }
   return false;
}

void SimpleCAN::printByteArrayHex(byte *data, int length) // prints 8-bit data in hex
{
   for (size_t i = 0; i < (size_t)length; i++)
   {
      Serial.print(data[i], HEX);
      Serial.print(' ');
   }
}

int SimpleCAN::PrintList(int listID)
{
   MSGDATALIST* list = FindList(listID);
   if(list == NULL)
   {
      return -1;
   }

   Serial.println("=== PRINTING LIST ===");

   Serial.print("ListID: ");
   Serial.println(list->listID);

   Serial.print("ExpectedListSize: ");
   Serial.println(list->expectedListSize);

   Serial.print("ListSize: ");
   Serial.println(list->listSize);

   Serial.print("totalDataSize: ");
   Serial.println(list->totalData);

   Serial.print("LastWritingTime (ms): ");
   Serial.println(list->lastWritingTime);

   MSGDATA* currentData = list->dataList;
   while (currentData != NULL)
   {
      Serial.println();
      Serial.print("Data ID: ");
      Serial.println(currentData->dataID);

      Serial.print("Data size: ");
      Serial.println(currentData->size);

      Serial.print("Data: ");
      printByteArrayHex(currentData->data, currentData->size);
      Serial.println();

      currentData = currentData->next;
   }
   Serial.println("=== === === === === ===");
   return 0;
}

int SimpleCAN::CreateNewList(int listID, int expectedListSize)
{
   MSGDATALIST *newList = new MSGDATALIST;
   newList->listID = listID;
   newList->listSize = 0;
   newList->totalData = 0;
   newList->dataList = NULL;
   newList->expectedListSize = expectedListSize;
   newList->lastWritingTime = millis();

   return InsertNewList(newList);
}

int SimpleCAN::InsertNewList(MSGDATALIST* list)
{
   if(FindList(list->listID) != NULL)
   {
      Serial.println("List already exists.");
      return -1;
   }
   MSGDATALIST* oldFirstList = lists;
   lists = list;
   list->next = oldFirstList;
   list->lastWritingTime = millis();
   return 0;
}

MSGDATALIST* SimpleCAN::FindList(int listID)
{
   // Serial.print("Finding a list with listID: ");
   // Serial.println(listID);
   MSGDATALIST* currentList = lists;
   while(currentList != NULL)
   {
      if(currentList->listID == listID)
      {
         // Serial.println("List found.");
         return currentList;
      }
      currentList = currentList->next;
   }
   // Serial.println("List has not been found");
   return NULL;
}

MSGDATALIST *SimpleCAN::FindListWithPreviousList(int listID, MSGDATALIST** previousList) // to do opschonen
{
   MSGDATALIST *_previousList = NULL;
   MSGDATALIST *currentList = lists;
   while (currentList != NULL)
   {
      if (currentList->listID == listID)
      {
         *previousList = _previousList;
         return currentList;
      }
      _previousList = currentList;
      currentList = currentList->next;
   }
   return NULL;
}

int SimpleCAN::InsertMsgData(msgDataList* list, MSGDATA* data)
{
   if(list == NULL || data == NULL)
   {
      return -1;
   }
   if(data->dataID < 0)
   {
      return -2;
   }
   if(list->dataList == NULL)
   {
      list->dataList = data;
   }
   else
   {
      MSGDATA* currentData = list->dataList;
      while (currentData != NULL)
      {
         if(currentData->dataID == data->dataID)
         {
            return -3;
         }
         if(currentData->next != NULL)
         {
            if(currentData->dataID < data->dataID && currentData->next->dataID > data->dataID)
            {
               MSGDATA* tempNext = currentData->next;
               currentData->next = data;
               data->next = tempNext;
               break;
            } 
         }
         else
         {
            currentData->next = data;
            break;
         }
         currentData = currentData->next;
      }
   }
   list->totalData+=data->size;
   list->listSize++;
   list->lastWritingTime = millis();
   return 0;
}

int SimpleCAN::AddMsgData(int listID, MSGDATA *newData, byte** allData, int *allDataSize)
{
   MSGDATALIST* list = FindList(listID);
   if (list == NULL)
   {
      return -1;
   }
   if (newData == NULL || newData->dataID < 0 || newData->dataID > list->expectedListSize)
   {
      return -2;
   }

   int insertResult = InsertMsgData(list, newData) != 0;
   if(insertResult == -2)
   {
      return -2;
   }
   if (insertResult == -3)
   {
      return -3;
   }
   
   if(list->expectedListSize == list->listSize)
   {
      if(CompileListData(list, allData, allDataSize) == 0)
      {
         // Serial.print("CompiledListData: ");
         // printByteArrayHex(*allData, *allDataSize);
         // Serial.println();
         // Serial.print("AllData address: ");
         // int allDataPtr = (int)allData;
         // Serial.println(allDataPtr);
         // Serial.print("AllDataSize address: ");
         // int allDataSizePtr = (int)allDataSize;
         // Serial.println(allDataSizePtr);
         // Serial.println("Removing list");

         RemoveList(listID);
         return 1;
      } 
   }
   return 0;
}

int SimpleCAN::CompileListData(MSGDATALIST *list, byte** allData, int* allDataSize)
{
   // Serial.println("====== IN COMPILE ======");

   if(list == NULL)
   {
      return -1;
   }

   byte *newArr = new byte[list->totalData];
   *allData = newArr;

   MSGDATA* currentData = list->dataList;
   
   size_t allDataIndex = 0;
   while (currentData != NULL)
   {
      // Serial.print("Compiling data: ");
      // printByteArrayHex(currentData->data, currentData->size);
      // Serial.println();
      for (size_t i = 0; i < (size_t)currentData->size; i++)
      {
         newArr[allDataIndex + i] = currentData->data[i];
      }
      allDataIndex += currentData->size;

      // Serial.print("Compiled data so far: ");
      // printByteArrayHex(newArr, allDataIndex);
      // Serial.println();
      
      currentData = currentData->next;
   }
   *allDataSize = list->totalData;
   if(list->expectedListSize != list->listSize)
   {
      return -2;
   }
   return 0;
}
int SimpleCAN::RemoveOldLists()
{
   // Serial.println("Removing old lists.");
   int listsRemoved = 0;
   MSGDATALIST *currentList = lists;
   while (currentList != NULL)
   {
      if(currentList->lastWritingTime + extendedMessageTimeout > millis())
      {
         RemoveList(currentList->listID);
         listsRemoved++;
      }

      currentList = currentList->next;
   }
   return listsRemoved;
}

int SimpleCAN::RemoveItemsInList(MSGDATALIST* list)
{
   if(list == NULL)
   {
      return -1;
   }
   MSGDATA *currentData = list->dataList;
   while (currentData != NULL)
   {
      MSGDATA* tempNext = currentData->next;

      delete[] currentData->data;
      delete currentData;

      currentData = tempNext;
   }
   return 0;
}

int SimpleCAN::RemoveList(int listID)
{
   MSGDATALIST* previousList = NULL;
   MSGDATALIST *list = FindListWithPreviousList(listID, &previousList);
   if (list == NULL)
   {
      return -1;
   }

   if(previousList == NULL) // list is first in list of lists
   {
      lists = list->next;
   }
   else
   {
      previousList->next = list->next;
   }

   RemoveItemsInList(list);
   delete list;
   return 0;
}
