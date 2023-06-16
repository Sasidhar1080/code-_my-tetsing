int n;
void setup() {
Serial.begin(115200);
n= analogread(A0);
pinMode(16,OUTPUT);
pinMode(5,OUTPUT);
}

void loop() {
  Serial.println(n);
  if(n<300)
  {
   digitalWrite(16,HIGH);
   digitalWrite(5,HIGH);
  }

}
