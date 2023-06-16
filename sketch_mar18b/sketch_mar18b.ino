int sensorPin = 34;   // select the input pin for the potentiometer    // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  double sumValue = 0.0;
  double sensitivity = 0.045;
  double outputValue;
  float zeroValue=0;
  for (int i=0; i<1000;i++)
  {
     sensorValue = analogRead(sensorPin);
     outputValue = (sensorValue *(3.27/4095))-(zeroValue);
     sumValue = (outputValue * outputValue) + (sumValue);
    delay(2);
    }
sumValue = (sumValue / 1000);
sumValue =sqrt(sumValue);
outputValue= sumValue/sensitivity;
Serial.print("processed voltage= : ");
Serial.println(sumValue);
Serial.print("current value =: ");
Serial.println(outputValue);
Serial.println("");
 }