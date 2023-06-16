#include <WiFi.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define ssid "esw-m19@iiith"
#define password "e5W-eMai@3!20hOct"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  timeClient.setTimeOffset(19800);
  // Wait for time to be updated
  while(!timeClient.update()) {
    
    timeClient.forceUpdate();
    Serial.println("Waiting for time sync...");
    delay(2000);
  }
}

void loop() {
  // Get current date and time
  String formattedTime = timeClient.getFormattedTime();
  
  // Print current date and time
  Serial.print("Current date and time: ");
  Serial.println(formattedTime);
  
  delay(1000);
}
