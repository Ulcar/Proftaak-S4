// Sources:
// Example code MCP2515 library

#ifndef SIMPLECAN_H
#define SIMPLECAN_H

#include <stdlib.h>
#include "MCP2515.h"

typedef struct msgData
{
  int dataID;           // The number of this item in the list
  byte *data;           // Data that came from a CANMSG
  int size;             // Size of data
  struct msgData *next; // Next item in list
} MSGDATA;

typedef struct msgDataList
{
  MSGDATA *dataList;
  int listID;                     // Unique ID of the list
  int listSize;                   // How many MSGDATA items are in the list
  int expectedListSize;           // How many MSGDATA items have to be in the list for it to return a full message
  int totalData;                  // How many bytes are saved in the list so far
  unsigned long lastWritingTime;  // Last time a new item has been added to the list in milliseconds
  struct msgDataList *next;       // Next item in metalist
} MSGDATALIST;

class SimpleCAN
{
  public:
    SimpleCAN(void (*fun_ptr_arr[])(CANMSG), int startValue, int size);

    // canTimeout is how many milliseconds to wait on the CANDIY shield for a new message
    // UniqueSenderID is for sending extended messages, the ID has to be unique on the network, range: 0-255
    SimpleCAN(int canTimeout);

    void Setup();

    // Basic message sending and receiving
    bool SendCANMSG(CANMSG msg);                                                 // Send message using the CANMSG struct.
    bool SendByteMessage(byte buffer[], int size, unsigned long adrsValue);      // Send message with a byte array, size of byte array and the address to send to.
    bool SendCharMsg(char buffer[], int size, unsigned long address);            // Send message with a char array, size of byte array and the address to send to.
    bool SendUint32(uint32_t val, unsigned long address);                        // Send a uint32 to address
    bool SendInt(int val, unsigned long address);                                // Send an int to address
    bool SendBool(bool val, unsigned long address);                              // Send a boolean to address
    bool SendString(char buffer[], int size, unsigned long adrsValue, int senderID); // Send message with a char array longer than 8 bytes, size of char array and the address to send to. Maximum size of string is 255*6 chars.
    bool SendLongByteMessage(byte buffer[], int size, unsigned long adrsValue, int senderID); // Send message with an array longer than 8 bytes, size of byte array and the address to send to. Maximum size of string is 255*6 chars.

    bool GetReceivedMessage(CANMSG *msg);                                        // Receive message through a pointer. False if no message is received.
    bool GetReceivedMessageWithAddress(CANMSG *msg, unsigned long *address);     // Receive message through a pointer and get the address. False if no message is received.


    // Conversions of types
    uint32_t CANMSGToUint32(CANMSG msg);                                    // Get a uint32 from a CANMSG struct.
    CANMSG Uint32ToCANMSG(uint32_t val);                                    // Put a uint32 in a CANMSG struct.
    CANMSG Uint32ToCANMSGWithAddress(uint32_t val, unsigned long address);  // Put a uint32 and an address value in a CANMSG struct. 

    int CANMSGToInt(CANMSG msg);                                            // Get an int from a CANMSG struct.
    CANMSG IntToCANMSG(int val);                                            // Put an int in a CANMSG struct.
    CANMSG IntToCANMSGWithAddress(int val, unsigned long address);          // Put an int and an address value in a CANMSG struct.

    bool CANMSGToBool(CANMSG msg);                                          // Get a bool from a CANMSG struct.
    CANMSG BoolToCANMSG(bool val);                                          // Put a bool in a CANMSG struct.
    CANMSG BoolToCANMSGWithAddress(bool val, unsigned long address);        // Put a bool and an address value in a CANMSG struct.


    void CANMSGToCharArray(CANMSG msg, char* str, int* size);                // Returns the char array and size of the array as pointers.
    bool CANMSGToString(CANMSG msg, char* str, int* size);                   // Receives CANMSG structs, will return TRUE once it has a full message, returning the string as a char pointer with the string size.
    bool CANMSGToByteArray(CANMSG msg, byte** arr, int *size);                // Receives CANMSG structs, will return TRUE once it has a full message, returning the bytes in a pointer together with the array size.

    // Misc functions
    void printByteArray(byte *arrayPtr, int size);                           // Print a byte array, easy way to print CANMSG data by entering msg->data and msg->dataLength

    // prints an array in hex on a single line, without ending the line
    void printByteArrayHex(byte *data, int length);

    // Prints list that holds received ExtendedMessage data
    // returns 0 on success
    // returns -1 if listID is invalid
    int PrintList(int listID);

  private : 
    MCP2515 can;
    int startValue;
    int functionArraySize;
    int canTimeout;
    void (**address_array)(CANMSG);

   bool BuildCanString(char buffer[], int size, unsigned long adrsValue);
   bool SendPartOfLongByteMessage(byte buffer[], int size, byte uniqueMessageID, byte messageNumber, unsigned long adrsValue);

   // Extended message handling

   long extendedMessageTimeout; // Time before old messages are removed with RemoveOldMessages() function
   MSGDATALIST* lists;
   



   // returns 0 on success
   // returns -1 on failure of creating a new list
   int CreateNewList(int listID, int expectedListSize);

   // returns 0 on success
   // returns -1 if this listID is already in use
   int InsertNewList(MSGDATALIST *list);

   // returns a list if found
   // returns NULL if the list is not found
   MSGDATALIST* FindList(int listID);

   // returns a list if found
   // returns NULL if the list is not found
   MSGDATALIST *FindListWithPreviousList(int listID, MSGDATALIST **previousList);

   // returns 0 on succes
   // returns -1 if msgDataList or data is NULL
   // returns -2 if data  is invalid
   // returns -3 if data with the same dataID already exists 
   int InsertMsgData(msgDataList* list, MSGDATA* data);

   // returns 1 when full extendedMessage has been received, it will be returned through pointers and the list will be removed
   // returns 0 when data has been added to list
   // returns -1 if this list does not exist yet
   // returns -2 if newData is NULL or invalid
   // returns -3 if data with that ID already exists in the list
   int AddMsgData(int listID, MSGDATA *newData, byte** allData, int* allDataSize);

   // returns 0 on success
   // returns -1 if list is NULL
   // returns -2 if the list is not complete yet and extendedMessage has not been fully received, this will still compile the list and return it
   int CompileListData(MSGDATALIST *list, byte **allData, int *allDataSize);

   // This function checks the lastAccessTime of each list, if long ago enough it gets removed as last data is not expected to arrive anymore
   // This function should be called periodically to clean up any lost messages
   // returns the amount of lists removed
   int RemoveOldLists(); 

   // returns 0 if list successfully cleaned from MSGDATA objects
   // returns -1 if listID is invalid
   int RemoveItemsInList(MSGDATALIST *list);

   // returns 0 if list successfully removed
   // returns -1 if listID is invalid
   int RemoveList(int listID); 
};

#endif