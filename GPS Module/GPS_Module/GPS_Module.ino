 #include "Adafruit_GPS.h" 
 #include "SimpleCAN.h"
// this sketch will allow you to bypass the Atmega chip
// and connect the GPS directly to the USB/Serial
// chip converter.
 
// Connect VIN to +5V
// Connect GND to Ground
// Connect GPS RX (data into GPS) to Digital 0
// Connect GPS TX (data out from GPS) to Digital 1
#define GPSSerial Serial
Adafruit_GPS GPS(&GPSSerial);
CANMSG CanMsg; 
SimpleCAN* can;
uint32_t timer = 0;
void setup() 
{
  can = new SimpleCAN();
  can->Setup();
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);  
}
void loop() 
{
  
  //Reading GPS Data
  if (GPS.newNMEAreceived()) {
    Serial.println(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA()))
      return; 
  }
  

  //Parsing GPS Data
  if (millis() - timer > 2000) {
    
    timer = millis();
    Serial.print("Location: ");
    //Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    Serial.print(", ");
    //Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);  
  }

  //Received Message Handling
  if (can->HandleReceivedMessage(&CanMsg)) 
  {
    //Message received
    Serial.print("Local time: ");
    Serial.print(millis());
    Serial.println(",");
    Serial.print("GPS Time: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.print("\nCan Message: "); 
  }
}

//PLACEHOLDER
void Emergency() 
{
    
}
