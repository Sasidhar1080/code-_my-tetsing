#include <dummy.h>

#include "arduino.h"
#include "HardwareSerial.h"
#include "WiFi.h"
#include "Wire.h"
#include "ThingSpeak.h"
const char ssid[] = "SCRC_LAB";

const char password[] = "Scrciiith@123";


#define  ADDR_6700 0x15 //T6713 i2C Address

long prevMillisThingSpeak = 0;

int intervalThingSpeak = 20000; // Minimum ThingSpeak write interval is 15 seconds
int data [4];
int ppmValue;
WiFiClient client;

void setup() {
  pinMode(21,INPUT_PULLUP);
  pinMode(22,INPUT_PULLUP);
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.println("14CORE | T67XX / AN161 CO2 Sensor Test Code");

      Serial.println("Initializing...............................");

      delay(2000);

      Serial.println("Starting i2C Communicate ADDR 0x15.........");

      Wire.begin();

      delay(1000); 

    Serial.println();

    Serial.println("Send Sensor's Data to ThingSpeak Using ESP32");

    Serial.println();

     WiFi.mode(WIFI_STA);

    ThingSpeak.begin(client);  // Initialize ThingSpeak

}
int readSensor(){

  byte data [4];

        float CO2ppmValue;

        // start I2C

        Wire.beginTransmission(ADDR_6700);

        Wire.write(0x04); Wire.write(0x13); Wire.write(0x8B); Wire.write(0x00); Wire.write(0x01);

        // end transmission

        Wire.endTransmission();

        // read report of current gas measurement in ppm

        delay(2000);

        Wire.requestFrom(ADDR_6700, 4);    // request 4 bytes from slave device

        data[0] = Wire.read();
        Serial.println(data[0]);
        data[1] = Wire.read();
        Serial.println(data[1]);
        data[2] = Wire.read();
        Serial.println(data[2]);
        data[3] = Wire.read();
        Serial.println(data[3]);
        return  ((data[2] & 0x3F ) << 8) | data[3];

}

void loop() {
  // put your main code here, to run repeatedly:
   delay(2000);
  
    int ppmValue = readSensor();

    Serial.println(ppmValue);
    delay(2000);         //changing temperature parameter to celsius

    delay(2000);
  }
