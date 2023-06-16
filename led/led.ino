#include<arduino.h>
#include<led.h>
int a=7; 
int b=6; 
int c=5; 
int d=11; 
int e=10; 
int f=8; 
int g=9; 
int dp=4; 

void display3(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);
    digitalWrite(g,HIGH);
} 

// display number5
void display5(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
  digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);
} 

void setup() 
{ 
    int i;
    for(i=4;i<=11;i++) 
        pinMode(i,OUTPUT);
} 
void loop() 
{     
    while(1) 
    {   clearDisplay();
  display0(); 
        delay(2000); 
        
        display3(); 
        delay(2000);
        clearDisplay();
        
        display5(); 
        delay(2000);
        clearDisplay(); 
         
    }
}
