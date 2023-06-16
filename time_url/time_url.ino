#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


 #define ssid "IIIT-Guest"         // network guest testing 
 #define password "I%GR#*S@!"

//const char* ssid = "IIIT-Guest";
//const char* password = "I%GR#*S@!";

WiFiUDP ntpUDP;
NTPClient timeClient1(ntpUDP, "pool.ntp.org");
NTPClient timeClient2(ntpUDP, "time.iiit.ac.in");
NTPClient timeClient3(ntpUDP, "time.google.com");


void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize and set the NTP time server
  timeClient1.begin();
  timeClient1.setTimeOffset(0);  // Set time offset (in seconds) if necessary

    timeClient2.begin();
  timeClient2.setTimeOffset(0);  // Set time offset (in seconds) if necessary
  
    timeClient3.begin();
  timeClient3.setTimeOffset(0);  // Set time offset (in seconds) if necessary
}

void loop() {
  // Update the current time
  timeClient1.update();
  timeClient2.update();
  timeClient3.update();
  // Get the current time in various formats 1
  Serial.print("Current Time: ");
  Serial.println(timeClient1.getFormattedTime());
  Serial.print("Epoch Time: ");
  Serial.println(timeClient1.getEpochTime());
  Serial.println("-----------------------");


  // Get the current time in various formats 1
  Serial.print("Current Time: ");
  Serial.println(timeClient2.getFormattedTime());
  Serial.print("Epoch Time: ");
  Serial.println(timeClient2.getEpochTime());
  Serial.println("-----------------------");

    // Get the current time in various formats 1
  Serial.print("Current Time: ");
  Serial.println(timeClient3.getFormattedTime());
  Serial.print("Epoch Time: ");
  Serial.println(timeClient3.getEpochTime());
  Serial.println("-----------------------");
  delay(1000);
}
