#include <Wire.h>
#include <Sodaq_SHT2x.h>
 
void setup()
{
Wire.begin();
Serial.begin(9600);
}
 
void loop()
{
Serial.print("Humidity(%RH): ");
Serial.println(SHT2x.GetHumidity());
Serial.print(" Temperature(C): ");
Serial.println(SHT2x.GetTemperature());
Serial.print(" Dewpoint(C): ");
Serial.println(SHT2x.GetDewPoint());
 
delay(1000);
}
