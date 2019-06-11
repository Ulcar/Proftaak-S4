#include <Arduino.h>
#include <SPI.h>
#include <MCP2515.h>
#include <SimpleCAN.h>
int test  = 0;
SimpleCAN canInterface(100);

bool receiveSerial(byte messageArray[], int *length);
bool sendSerial(byte message[256], int length);
bool sendCan(byte message[], int length);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}



void loop() {
byte array[257];
  int l = 0;
bool recieved = receiveSerial(array, &l);
  if(recieved){
    sendCan(array, l);
  }
  //Serial.write(2);
  //delay(5000);


}

bool sendSerial(byte message[], int length)
{

  Serial.write('#');
  for (int i = 0; i < length; i++)
  {
    Serial.write(message[i]);
  }
  Serial.write('$');
  return true;
}

bool sendCan(byte message[], int length)
{
  //byte test = 2;
  //Serial.write(test);
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

  //Serial.write(address);
  char canMessage[256];
  int canMessageLength = 0;

  while(true)
  {
    if(i == length)break;
    canMessage[canMessageLength] = message[i];
    //Serial.write(message[i]);
    //Serial.print(canMessage[canMessageLength]);

    canMessageLength++;
    i++;
  }


  canInterface.SendCharMsg(canMessage, canMessageLength, address);
  return true;
}

bool receiveSerial(byte messageArray[], int *length)
{
  *length = 0;
  if(Serial.available() && Serial.read() == '#')
  {
    while (true)
    {
      while (Serial.available() == 0);
      {
        //do nothing, just wait
      }
      byte incoming = Serial.read();
      if(incoming == '$')
      {
        break;
      }
      //Serial.write(incoming);
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
