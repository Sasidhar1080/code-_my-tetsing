
void setup()
{
pinMode(13,OUTPUT);
pinMode(12,OUTPUT);
pinMode(11,OUTPUT);
pinMode(10,OUTPUT);
pinMode(9,OUTPUT);
pinMode(8,OUTPUT);
pinMode(7,OUTPUT);
pinMode(4,OUTPUT);  
}
void loop()
{
digitalWrite(13,HIGH); 
digitalWrite(12,HIGH);
digitalWrite(11,HIGH);
digitalWrite(10,HIGH);
digitalWrite(9,LOW);
digitalWrite(8,LOW);
digitalWrite(7,HIGH);//1
digitalWrite(4,LOW);
   
 delay(2000);
digitalWrite(13,HIGH); 
digitalWrite(12,HIGH);
digitalWrite(11,HIGH);
digitalWrite(10,HIGH);
digitalWrite(9,LOW);
digitalWrite(8,LOW);//1
digitalWrite(7,HIGH);
digitalWrite(4,LOW);
   
 delay(2000);
  digitalWrite(13,LOW); 
digitalWrite(12,HIGH);
digitalWrite(11,HIGH);
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(8,LOW);//9
digitalWrite(7,LOW);
digitalWrite(4,LOW);
   
 delay(2000);
digitalWrite(13,HIGH); 
digitalWrite(12,HIGH);
digitalWrite(11,HIGH);
digitalWrite(10,HIGH);
digitalWrite(9,LOW);
digitalWrite(8,LOW);//1
digitalWrite(7,HIGH);
digitalWrite(4,LOW);
   
 delay(2000); 
digitalWrite(13,LOW); 
digitalWrite(12,LOW);
digitalWrite(11,HIGH);
digitalWrite(10,HIGH);
digitalWrite(9,HIGH);
digitalWrite(8,LOW);//1
digitalWrite(7,LOW);
digitalWrite(4,LOW);
 
 delay(2000);
  
digitalWrite(13,LOW); 
digitalWrite(12,LOW);
digitalWrite(11,LOW);
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(8,LOW);//0
digitalWrite(7,LOW);
digitalWrite(4,LOW);
   
 delay(2000);
digitalWrite(13,LOW); 
digitalWrite(12,HIGH);
digitalWrite(11,HIGH);
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(8,LOW);//3
digitalWrite(7,LOW);
digitalWrite(4,LOW);
   
 delay(2000);
digitalWrite(13,LOW); 
digitalWrite(12,LOW);
digitalWrite(11,HIGH);
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(8,HIGH);//5
digitalWrite(7,LOW);
digitalWrite(4,LOW);
   
 delay(2000);
digitalWrite(13,LOW); 
digitalWrite(12,LOW);
digitalWrite(11,HIGH);
digitalWrite(10,LOW);
digitalWrite(9,LOW);
digitalWrite(8,HIGH);//5
digitalWrite(7,LOW);
digitalWrite(4,LOW);
   
 delay(2000);
}
  
