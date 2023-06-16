#include <IRremote.h>
int RECV_PIN = 15;
IRrecv irrecv(RECV_PIN);
decode_results results;
bool received = false;

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn(); 
}

void loop() 
{
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); 
    received = true;
  }
  
  if (received) {
    // Do something with the received IR data in boolean form
    if (results.value == 0xFF6897) {
      Serial.println("IR signal received: true");
      // Do something when the IR signal is true
    } else {
      Serial.println("IR signal received: false");
      // Do something when the IR signal is false
    }
    received = false;
  }
}
