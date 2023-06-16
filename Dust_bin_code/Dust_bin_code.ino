#include <Servo.h>

Servo servoMain; // Define our Servo

int IRSensor = 2; // connect ir sensor to arduino pin 2

int LED1 = 13; // conect Led1 to arduino pin 13
int LED2 = 12;//connect Led 2 to arduino pin 12
//Defiene ultrasonic pins
int trigpin = 10;
int echopin = 11;
int distance;
float duration;
float cm;

void setup() 
{

  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (LED1, OUTPUT); // Led pin OUTPUT
  
  servoMain.attach(9); // servo on digital pin 10

  pinMode(trigpin, OUTPUT);
  
  pinMode(echopin, INPUT);
  Serial.begin(9600);
  
}

void loop()
{

  // ultrasonic Calculation

      
    digitalWrite(trigpin, LOW);
    
    delay(2);
    
    digitalWrite(trigpin, HIGH);
    
    delayMicroseconds(10);
    
    digitalWrite(trigpin, LOW);
    
    duration = pulseIn(echopin, HIGH);
    
    cm = (duration/58.82);
    
    distance = cm;

  //ir sensor read data
    
  int statusSensor = digitalRead (IRSensor);
  
  if (statusSensor == 1)
      {
        digitalWrite(LED1, LOW); // LED LOW
        servoMain.write(90); // Turn Servo back to center position (90 degrees)
    
        delay(3000);
      }
    //quantity measure 

  else if(distance<10)
      {
         digitalWrite(LED1, LOW); // LED ON
         delay(5000);
      }
   // stay calm
  else 
      { 
        servoMain.write(0);
        
        delay(50);
        digitalWrite(LED2, HIGH); // LED High
       
      }
}
