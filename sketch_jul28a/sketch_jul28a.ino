

#if (defined(__AVR__))
#include <avr\pgmspace.h>
#else
#include <pgmspace.h>
#endif
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

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

//ardfruit commands
//Adafruit_SH1106 display(21, 22);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void setup() {

pinMode(F,INPUT);
pinMode(B,INPUT);
pinMode(OLED_SDA, INPUT_PULLUP); // set pin to input
pinMode(OLED_SCL, INPUT_PULLUP); // set pin to input
Serial.begin(9600);
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
display.begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
display.display();
delay(50);
display.clearDisplay();
display.setCursor(0,0);
display.fillRoundRect(0, 2, 128, 64, 8, WHITE);



display.setCursor(30,10);
display.setTextSize(2.5);
display.setTextColor(BLACK);
display.println("COUNT:");

display.setCursor(50,40);
display.setTextSize(2);
display.setTextColor(BLACK);
display.println(count);
display.display();

delay(5);


}
