#include "WiFi.h"
#include "ThingSpeak.h"

#define ssid ""
#define pass ""

unsigned long channel =
const char *apik=
int sensorPin = 34; 
int sensorValue = 0; 
double sumValue = 0.0;
double sensitivity = 0.045;
double outputValue;
float zeroValue=0; 

WiFiClient client;

void setup() 
{
  pinMode(sensorPin,INPUT);
  Serial.begin(115200);
  ThinkSpeak.begin(client)
}

#define CURRENT_PIN 34
#define SENSITIVITY 0.045

float zv(){
  analogReadResolution(12);
  delay(1000);
  for(int i=0; i<500;i++){
  sensorValue = analogRead(CURRENT_PIN);
  Serial.print("Analog reading:");
  Serial.println(sensorValue);
  zeroValue = (sensorValue *(3.27/4095)) + (zeroValue);
  delay(2);
  }

  zeroValue = zeroValue /500;
  Serial.println("**************************************");
     
  Serial.print("Zero Value when no Load condition =: ");
  Serial.println(zeroValue);
  return zeroValue;
}

float read_sensor(float zeroValue){
  double sumValue = 0.0;
// capturing sensor voltage rms and current

  for(int i=0; i<1000;i++){

  sensorValue = analogRead(CURRENT_PIN);
  outputValue = (sensorValue *(3.27/4095)) - ( zeroValue);

  sumValue = (outputValue * outputValue) + (sumValue);
  delay(2);

  }
  sumValue = (sumValue / 1000);
  sumValue = sqrt(sumValue);
  outputValue = sumValue /SENSITIVITY;

  Serial.print("Processed Sensor Voltage Value in Volts =:   ");
  Serial.println(sumValue);
  Serial.print("Current  Value in Amp =: ");
  Serial.println(outputValue);
  Serial.println("____________________________________");
  return outputValue;
}

void loop()
{
  float zValue=zv();
  
  ThingSpeak.setField(1,outputValue);
  int x = ThingSpeak.writeFields(channel,apik);
  if (x==200)
  {
    Serial.println("Channel updated");

  }
  
}
