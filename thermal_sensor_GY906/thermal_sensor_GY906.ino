#include <Wire.h>
#include <Adafruit_MLX90614.h>
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define SCl 22
#define SDA 21
void setup()
{

pinMode(SDA,INPUT);
pinMode(SCL,INPUT);
Serial.begin(9600);
mlx.begin();
}
 
void loop()
{


Serial.print("*C\tObject = ");
Serial.print(mlx.readObjectTempC());
Serial.println("*C");


Serial.print("*F\tObject = ");
Serial.print(mlx.readObjectTempF());
Serial.println("*F");
Serial.println();
delay(5000);
}
