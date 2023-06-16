int led_pin = 9;
void setup() {
  
  pinMode(led_pin, OUTPUT);
}
void loop() {
  //Fading the LED
  for(int i=0; i<64; i++){
    analogWrite(led_pin, i);
    delay(5);
  }
  for(int i=64; i<128; i++){
    analogWrite(led_pin, i);
    delay(5);
  }
    for(int i=128; i<192; i++){
    analogWrite(led_pin, i);
    delay(5);
  }

     for(int i=192; i<255; i++){
    analogWrite(led_pin, i);
    delay(5);
  }
}
   
   
