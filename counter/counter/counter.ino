
const int CLOCK = 3;

void setup() {
  pinMode(CLOCK, OUTPUT);
}

void loop() {
    digitalWrite(CLOCK, HIGH);
    digitalWrite(CLOCK, LOW);
    delay(1000);

}
