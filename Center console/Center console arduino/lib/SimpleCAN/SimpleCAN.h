// Sources:
// Example code MCP2515 library

#ifndef SIMPLECAN_H
#define SIMPLECAN_H

#include "MCP2515.h"

class SimpleCAN
{
  public:
    SimpleCAN(void (*fun_ptr_arr[])(CANMSG), int startValue, int size);
   SimpleCAN(int canTimeout);

    void Setup();

    // Basic message sending and receiving
    bool SendCANMSG(CANMSG msg);                                                       // Send message using the CANMSG struct.
    bool SendByteMessage(byte buffer[], int size, unsigned long address);                 // Send message with a byte array, size of byte array and the address to send to.
    bool SendCharMsg(char buffer[], int size, unsigned long address);                     // Send message with a char array, size of byte array and the address to send to.
    bool SendUint32(uint32_t val, unsigned long address);                                 // Send a uint32 to address
    bool SendInt(int val, unsigned long address);                                         // Send an int to address
    bool SendBool(bool val, unsigned long address);                                       // Send a boolean to address
    bool SendString(char buffer[], int size, uint8_t uniqueID, unsigned long adrsValue);  // Send message with a char array longer than 8 bytes, size of byte array, unique string ID who the message is from and the address to send to. Maximum size of string is 255*6 chars.

    bool GetReceivedMessage(CANMSG *msg);                                                 // Receive message through a pointer. False if no message is received.
    bool GetReceivedMessageWithAddress(CANMSG *msg, unsigned long *address);              // Receive message through a pointer and get the address. False if no message is received.


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


    // Misc functions
    void printByteArray(byte *arrayPtr, int size);                           // Print a byte array, easy way to print CANMSG data by entering msg->data and msg->dataLength

  private : MCP2515 can;
    int startValue;
    int functionArraySize;
    int canTimeout;
    void (**address_array)(CANMSG);
};

#endif
