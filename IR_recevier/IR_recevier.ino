
#include <IRremote.h>
 
#define IR_PIN 14
 
IRrecv irrecv(IR_PIN);
decode_results results;
 
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
 
void loop() {
  if (irrecv.decode(&results))
  {
    Serial.println(results.value , HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
