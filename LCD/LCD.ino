#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
 
  delay(4000);
}

void loop(){
  lcd.setCursor(0, 0);
  lcd.print("Sasidhar");
  delay(1000);
  
  lcd.setCursor(0, 1);
  lcd.print("11910355");
  delay(1000);
  }
