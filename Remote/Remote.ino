/*
 * IRremoteESP8266: IRServer - demonstrates sending IR codes controlled from a webserver
 * Version 0.3 May, 2019
 * Version 0.2 June, 2017
 * Copyright 2015 Mark Szabo
 * Copyright 2019 David Conran
 *
 * An IR LED circuit *MUST* be connected to the ESP on a pin
 * as specified by kIrLed below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif  // ESP8266
#if defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#endif  // ESP32
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>

const char* kSsid = "Spidy Sasi";
const char* kPassword = "10.10.0.1";
MDNSResponder mdns;

#if defined(ESP8266)
ESP8266WebServer server(80);
#undef HOSTNAME
#define HOSTNAME "TechPosts IR Server"
#endif  // ESP8266
#if defined(ESP32)
WebServer server(80);
#undef HOSTNAME
#define HOSTNAME "esp32"
#endif  // ESP32

const uint16_t kIrLed = 4;  // ESP GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>" HOSTNAME " Demo </title>" \
                "<meta http-equiv=\"Content-Type\" " \
                    "content=\"text/html;charset=utf-8\">" \
                "</head>" \
                "<body>" \
                  "<h1>Control All IR Devices with " HOSTNAME ".</h1>" \
                      "<h2>RGB Board</h2>" \
                      "<p><a href=\"ir?code=16236607\"> RGB ON</a></p>" \
                      "<p><a href=\"ir?code=16203967\"> OFF</a></p>" \
                      "<p><a href=\"ir?code=16246807\"> RANDOM</a></p>" \
                      "<p><a href=\"ir?code=16195807\"> RED</a></p>" \
                      "<p><a href=\"ir?code=16228447\"> GREEN</a></p>" \
                      "<p><a href=\"ir?code=16228447\"> BLUE</a></p>" \
                      "<p><a href=\"ir?code=16244767\"> WHITE</a></p>" \
                      "<p><a href=\"ir?code=16208047\"> BLUE 1</a></p>" \
                      "<p><a href=\"ir?code=16216207\"> BLUE 2</a></p>" \
                      "<p><a href=\"ir?code=16206007\"> BLUE 3</a></p>" \
                      "<p><a href=\"ir?code=16214167\"> BLUE 4</a></p>" \
                      "<p><a href=\"ir?code=16224367\"> GREEN 1</a></p>" \
                      "<p><a href=\"ir?code=16232527\"> GREEN 1</a></p>" \
                      "<p><a href=\"ir?code=16222327\"> GREEN 1</a></p>" \
                      "<p><a href=\"ir?code=16230487\"> GREEN 1</a></p>" \
                      "<p><a href=\"ir?code=16191727\"> RED 1</a></p>" \
                      "<p><a href=\"ir?code=16199887\"> RED 1</a></p>" \
                      "<p><a href=\"ir?code=16189687\"> RED 1</a></p>" \
                      "<p><a href=\"ir?code=16197847\"> RED 1</a></p>" \
                      "<h2>Creative Stage Controls</h2>" \
                      "<p><a href=\"ir?code=83595375\"> Stage ON/OFF</a></p>" \
                      "<p><a href=\"ir?code=83603535\"> INPUT</a></p>" \
                      "<p><a href=\"ir?code=83593335\"> RESET</a></p>" \
                      "<p><a href=\"ir?code=83591295\"> BT-PAIR</a></p>" \
                      "<h3>Equalizers</h3>" \
                      "<p><a href=\"ir?code=83605575\"> CINEMA</a></p>" \
                      "<p><a href=\"ir?code=83597415\"> MUSIC</a></p>" \
                      "<p><a href=\"ir?code=83601495\"> CONCERT</a></p>" \
                      "<p><a href=\"ir?code=83577015\"> GAMING</a></p>" \
                      "<h3>Bluetooth Controls</h3>" \
                      "<p><a href=\"ir?code=83621895\"> STOP</a></p>" \
                      "<p><a href=\"ir?code=83613735\"> PLAY/PAUSE</a></p>" \
                      "<p><a href=\"ir?code=83617815\"> NEXT</a></p>" \
                      "<p><a href=\"ir?code=83619855\"> PREVIOUS</a></p>" \
                      "<h3>Treble</h3>" \
                      "<p><a href=\"ir?code=83558655\"> INCREASE</a></p>" \
                      "<p><a href=\"ir?code=83607615\"> DECREASE</a></p>" \
                      "<h3>Bass</h3>"
                      "<p><a href=\"ir?code=83587215\"> INCREASE</a></p>" \
                      "<p><a href=\"ir?code=83581095\"> DECREASE</a></p>" \
                      "<h3>Volume</h3>" \
                      "<p><a href=\"ir?code=83583135\"> INCREASE</a></p>" \
                      "<p><a href=\"ir?code=83615775\"> DECREASE</a></p>" \
                      "<p><a href=\"ir?code=83589255\"> MUTE</a></p>" \
                "</body>" \
              "</html>");
}

void handleIr() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "code") {
      uint32_t code = strtoul(server.arg(i).c_str(), NULL, 10);
#if SEND_NEC
      irsend.sendNEC(code, 32);
#endif  // SEND_NEC
    }
  }
  handleRoot();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  irsend.begin();

  Serial.begin(115200);
  WiFi.begin(kSsid, kPassword);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(kSsid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());

#if defined(ESP8266)
  if (mdns.begin(HOSTNAME, WiFi.localIP())) {
#else  // ESP8266
  if (mdns.begin(HOSTNAME)) {
#endif  // ESP8266
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ir", handleIr);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
