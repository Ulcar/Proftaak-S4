#include "SimpleCAN.h"
enum VolumeOption 
{
  Decrease,
  Increase
};

enum Sources 
{
  Stuur,
  CC  
};

enum Outputs 
{
  Radio,
  Telefoon,
  Telemetrie  
};

int CurrentVolume = 0;
Outputs CurrentOutput = Radio;

CANMSG CanMsg; 
SimpleCAN* can;
uint32_t timer = millis();
void setup() 
{
  can = new SimpleCAN();
  can->Setup();
}

void loop() 
{
  
}

void GetCurrentAudioLevel(Sources Source) 
{
   switch (Source) 
   {
      case CC:
        break;
      case Stuur:
        break; 
   }
}

Outputs GetCurrentChannel() 
{
  return CurrentOutput;  
}

void SwitchChannel() 
{
    switch(CurrentOutput) 
    {
        case Radio:
          CurrentOutput = Telefoon;
          break;
        case Telefoon:
          CurrentOutput = Telemetrie;
          break;
        case Telemetrie:
          CurrentOutput = Radio;
          break;    
    }
}

void ChangeVolume(VolumeOption Option, Sources Source) 
{
  switch (Option) 
  {
    case Increase: 
    {
      if (CurrentVolume + 1 <= 16) { CurrentVolume += 1; }
      break;
    } 
    case Decrease: 
    {
      if (CurrentVolume - 1 >= 0) { CurrentVolume -= 1; }
      break;
    } 
  }  

  if (Source == Stuur) 
  {
      //Laat dit zien op de CC
  }
  if (Source == CC) 
  {
      //Do Nothing  
  }
} 
