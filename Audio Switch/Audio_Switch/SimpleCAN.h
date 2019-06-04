// Sources:
// Example code MCP2515 library

#ifndef SIMPLECAN_H
#define SIMPLECAN_H

#include "MCP2515.h"

class SimpleCAN
{
  public:
    SimpleCAN();

    bool HandleTextMsg(CANMSG msg);
    uint32_t HandleUint32Msg(CANMSG msg);
    void MakeUint32Msg(CANMSG *msg, uint32_t val);
    bool HandleReceivedMessage(CANMSG *msg);
    void printByteArray(byte *arrayPtr, int size);
    void Setup();
    void SendCanString(char buffer[], int size, unsigned long adrsValue);
    

  private:
    MCP2515 can;
    bool BuildCanString(char buffer[], int size, unsigned long adrsValue);
    int startValue;
    int functionArraySize;
    void (**address_array)(CANMSG);
};

#endif
