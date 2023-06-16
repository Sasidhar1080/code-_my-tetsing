#include <HTTPClient.h>
#include <WiFi.h>
#include "time.h"

#define WIFI_SSID "SCRC_LAB"
#define WIFI_PASS "Scrciiith@123"

#define CSE_IP      "onem2m.iiit.ac.in"
#define CSE_PORT    443
#define HTTPS       true
#define OM2M_ORGIN  "AirPoll@20:22uHt@Sas"
#define OM2M_MN     "/~/in-cse/in-name/"
#define OM2M_AE     "AE-AQ"
#define OM2M_DATA_CONT  "AQ-BN00-00/Data"

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Set time using NTP
  configTime(0, 0, "pool.ntp.org");
  Serial.println("Waiting for time sync...");
  while (time(nullptr) < 1000000000) {
    delay(1000);
  }
  Serial.println("Time synced!");

  // Set up the SSL client
  client.setInsecure();  // Ignore SSL verification
}

void loop() {
  // Make a GET request to the CSE
  String url = "http";
  if (HTTPS) {
    url += "s";
  }
  url += "://" + String(CSE_IP) + ":" + String(CSE_PORT) + String(OM2M_MN) + String(OM2M_AE) + "/" + String(OM2M_DATA_CONT);
  HTTPClient http;
  http.begin(client, url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("GET request failed");
  }

  http.end();

  delay(5000);
}
