#include <Arduino.h>
#include <SPI.h>
#include <MCP2515.h>
#include <SimpleCAN.h>
int test  = 0;
SimpleCAN canInterface(100);

bool receiveSerial(char messageArray[], int *length);
bool sendSerial(char message[256], int length);
bool sendCan(char message[], int length);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}



void loop() {
  char array[256];
  int l = 0;
  bool recieved = receiveSerial(array, &l);
  if(recieved){
    sendCan(array, l);
  }


}

bool sendSerial(char message[], int length)
{

  Serial.write('#');
  for (int i = 0; i < length; i++)
  {
    Serial.write(message[i]);
  }
  Serial.write('$');
  return true;
}

bool sendCan(char message[], int length)
{
  unsigned long address = 0;
  int i= 0;
  while(true)
  {
    if(message[i] == '%')
    {
      i++;
      break;

    }
    int temp = message[i] - '0';
    address = (address * 10) + temp;
    i++;
  }

  char canMessage[256];
  int canMessageLength = 0;

  while(true)
  {
    if(i == length)break;
    canMessage[canMessageLength] = message[i];
    Serial.println(canMessage[canMessageLength]);

    canMessageLength++;
    i++;
  }

  canInterface.SendCharMsg(canMessage, canMessageLength, address);
  return true;
}

bool receiveSerial(char messageArray[], int *length)
{
  *length = 0;
  if(Serial.available() && Serial.read() == '#')
  {
    while (true)
    {
      while (Serial.available() == 0);
      char incoming = Serial.read();
      if(incoming == '$')break;
      messageArray[*length] = incoming;
      *length = *length + 1;
      incoming = Serial.read();
    }

  }
  else
  {
    return false;
  }
  return true;

}
