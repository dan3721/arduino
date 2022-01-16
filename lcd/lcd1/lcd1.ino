#include "LCDIC2.h"

LCDIC2 lcd(0x27, 20, 4);
//LCDIC2 lcd(0x3F, 20, 4);

void setup() {
  Serial.begin(9600);
  while (! Serial); 
  Serial.println("SENT");

  if (lcd.begin()) {
    lcd.print("Hello, World!");
    //    lcd.setBacklight(false);
  }
}

void loop() {

//  String s1 = Serial.readStringUntil('\n');
//  lcd.print(s1);

  if (Serial.available() > 0) {
    String s1 = Serial.readString();
    s1.trim();
    lcd.clear();
    lcd.print(s1);
  }
  
}
