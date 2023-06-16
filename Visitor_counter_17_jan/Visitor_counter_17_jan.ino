#include <WiFi.h>
#include <WiFiClient.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


#define i2c_Address 0x3c

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define OLED_SDA 21
#define OLED_SCL 22


//code for thingspeak script down


#include <ThingSpeak.h>

const char* ssid = "SCRC_LAB";
const char* password = "Scrciiith@123";
const char* serv = "http://api.thingspeak.com";
// Service API Key
unsigned long Channel_ID = 1816757;  //replace with your Channel ID
const char * API_key = "8JQ08D54RO47H2NC";

WiFiClient client;

int incomingSensor = 18;
int outgoingSensor = 19;
int incomingCount = 0;
int outgoingCount = 0;
unsigned long lastIncomingSensor = 0;
unsigned long lastOutgoingSensor = 0;
const int debounceDelay = 100;

const char* host = "esp32";
// const char* ssid = "SCRC_LAB";
// const char* password = "Scrciiith@123";
int count=0;

/*


  /*
   setup function
*/
void setup(void) {

  //
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  //display.setContrast (0); // dim display
  pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP);           // set pin to input
  pinMode(incomingSensor, INPUT);
  pinMode(outgoingSensor, INPUT);
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 20);
  display.print("Visitor");
  display.setCursor(20, 40);
  display.print("Counter");
  display.display();
  delay(2000);

  Serial.println("Connected to the WiFi network");
  //
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
}

void loop(void) 
{

  if (digitalRead(incomingSensor) == LOW && count<20) 
  {
      while(digitalRead(outgoingSensor) == LOW )
       {
       count++;
      Serial.println(count);
      lastIncomingSensor = millis(); 
      displaycount();
       }
  }
   else if (digitalRead(outgoingSensor) == LOW && count>0 ) 
  {
    while (digitalRead(incomingSensor) == LOW )
      {
       count--;
       Serial.println(count);
      lastIncomingSensor = millis();
       displaycount(); 
      }
    //}
  }
  
}


  //Display visitor count
  void displaycount()
  {
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(15, 1);
    display.print("Current Visitors");
    display.setTextSize(2.5);
    display.setCursor(55, 15);
    display.print(count);
    display.display();
    delay(5);


  }
