
#include <Adafruit_SH1106.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SH1106 display(22, 21);

 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define SCl 22
#define SDA 21

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

void setup()
{
  pinMode(SDA,INPUT);
  pinMode(SCL,INPUT);
  Serial.begin(9600);
   mlx.begin();

  Serial.print("*C\tObject = ");
Serial.print(mlx.readObjectTempC());
Serial.println("*C");


Serial.print("*F\tObject = ");
Serial.print(mlx.readObjectTempF());
Serial.println("*F");
Serial.println();
delay(5000);
//temp
  pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP);           // set pin to input
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  
  //celsious
   display.setCursor(0,32);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("mlx.readObjectTempC()");

  // fH
   display.setCursor(64,32);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("mlx.readObjectTempF()");

  delay(5000);
}
 
void loop()
{
}
