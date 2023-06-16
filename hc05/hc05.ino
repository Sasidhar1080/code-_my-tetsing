void setup() {
pinMode(12,OUTPUT);
Serial.begin(9600);
}
void loop()
{
while(Serial.available())
{
delay(10);
char x = Serial.read();
Serial.println(x);

if(x =='1')
{
digitalWrite(12,HIGH);
delay(5000);
}
if(x='0')
{
digitalWrite(12,LOW);
delay(1000);
}}}
