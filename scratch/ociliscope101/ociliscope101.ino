

#include <Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600); 
  while(!Serial);
  myservo.attach(PD4);
}

void loop() {
  int val = analogRead(A0);
  Serial.print("val:");
  Serial.println(val);
  int duty = map(val, 0,1023,0,255);
  Serial.print(",duty:");
  Serial.print(duty);
  analogWrite(PD3, duty);
  int degree = map(val, 0, 1023, 0, 180); 
  myservo.write(degree); 
  Serial.print(",degree:");
  Serial.println(degree);
  delay(100);
}
