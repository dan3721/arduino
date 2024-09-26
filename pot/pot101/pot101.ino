void setup() {
  Serial.begin(9600);
}

void loop() {
  int i = analogRead(A0);
//  Serial.println(i);
  int y = map(i, 0, 1023, -100, 100);
  Serial.println(y);
  delay(1);
}
