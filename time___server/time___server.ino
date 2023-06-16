#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiServer.h>



 #define ssid "IIIT-Guest"         // 
 #define password "I%GR#*S@!"

//const char* ssid = "esw-m19@iiith";
//const char* password = "e5W-eMai@3!20hOct";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

WiFiServer server(80);  // Create a web server on port 80

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.begin();

  timeClient.begin();
}

void loop() {
  timeClient.update();

  WiFiClient client = server.available();
  if (client) {
    String response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    response += timeClient.getFormattedTime();

    client.print(response);
    delay(10);
    client.stop();
  }

  delay(1000);
}
