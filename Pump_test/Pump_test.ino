int pump1 = 16 ;                    //This is the output pin on the Arduino
int pump2 = 17 ;  
void setup() 

{  
  pinMode(pump1, OUTPUT);
 pinMode(pump2, OUTPUT);//Sets that pin as an output
  Serial.begin(115200);
}

void loop() 
{
  digitalWrite(pump1,LOW);
  Serial.println("motor high");//Switch Solenoid ON
  delay(5000);                          //Wait 1 Second
  digitalWrite(pump1, HIGH); 
  Serial.println("motor 1 low");//Switch Solenoid OFF
  delay(5000);                          //Wait 1 Second
 digitalWrite(pump2, LOW);      //Switch Solenoid ON
 delay(5000);
 Serial.println("motor 2 high");//Wait 1 Second
 delay(5000);                    //Wait 1 Second
 digitalWrite(pump2, HIGH);     //Switch Solenoid OFF Serial.println("hi");
 Serial.println("motor 2 low");//Wait 1 Second
  delay(5000);    
}
