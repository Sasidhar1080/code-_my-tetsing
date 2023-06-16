#include "OneWire.h"
#include "GravityTDS.h"

#define TdsSensorPin 35
GravityTDS gravityTds;

float temperature = 25,tdsValue = 0;

void setup()
{
 
    Serial.begin(115200);
    gravityTds.setPin(35);

    gravityTds.begin();  //initialization
}

void loop()
{

    tdsValue = gravityTds.getTdsValue();  // then get the value
    Serial.print(tdsValue,0);
    Serial.println("ppm");
    delay(1000);
}
