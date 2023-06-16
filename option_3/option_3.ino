#include <Adafruit_SH1106.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define OLED_SDA 27
#define OLED_SCL 26

Adafruit_SH1106 display(27, 26);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

const unsigned char picture [] PROGMEM = {

    };

    

 
void setup() {
  // put your setup code here, to run once:
  pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP);           // set pin to input
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // display IIITH logo
  display.clearDisplay();
  display.display();
  display.drawBitmap(0, 0,  picture , 128, 64, WHITE);
  display.display();
   delay(10000);

}
void loop() {
  // put your main code here, to run repeatedly:

}
