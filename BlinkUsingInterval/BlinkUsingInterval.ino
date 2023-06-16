// Blink the led every 1 second using setInterval

#include <AsyncTimer.h>
#define LED_BUITIN
AsyncTimer t;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  t.setInterval([]() { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); },
                1000);
}

void loop() { t.handle(); }
