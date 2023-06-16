/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
//define front,back,count varible
int F = 18;
int B = 19;
int count = 0;
int a=0;
int b=0;
int i=0;
int x;

//define oled pins
#define OLED_SDA 21
#define OLED_SCL 22



void setup()   {
  display.begin(i2c_Address, true);
  
  Serial.begin(9600);
  delay(250);
  
  
  display.display();
  
  
  display.begin(SH110X_SETCOMPINS, 0x3C);
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  // wait for the OLED to power up
   // Address 0x3C default
 //display.setContrast (0); // dim display
 
  
  // Clear the buffer.
  display.clearDisplay();
  pinMode(F,INPUT);
  pinMode(B,INPUT);
  pinMode(OLED_SDA, INPUT_PULLUP); // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP); // set pin to input
  display.display();

}


void loop() {
  

a=count;

//Enter Count
if(digitalRead(F)== 1 && count <25)
{
  delay(10);
  if(digitalRead(F) == 0)
    {
            count++;
            b=count;
            delay(100);
    }  
    
}

//Exit Count
if(digitalRead(B) ==1 && count >0)
{
  delay(10);
  if(digitalRead(B) == 0)
    {
            count--;
            b=count;
            delay(100);
    }  
    
}
     

else if(digitalRead(F) == 1  && digitalRead(B)== 1)
{
 delay(5);
  }
if(a!=b)
{
  displaycount();
  a=b;
}
}


//Display visitor count
void displaycount()
{

display.begin(SH110X_SETCOMPINS, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
display.display();
delay(50);
display.setCursor(0,0);




display.setCursor(30,10);
display.setTextSize(2.5);
display.setTextColor(SH110X_WHITE);
display.println("COUNT:");

display.setCursor(50,40);
display.setTextSize(2);
display.setTextColor(SH110X_WHITE);
display.println(count);
display.display();

delay(5);


}

