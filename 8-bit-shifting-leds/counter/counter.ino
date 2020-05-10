
const int DATA_PIN_OUT = 3;
const int CLOCK_PIN_OUT = 4;

const int DELAY = 20;

void setup() {
  pinMode(DATA_PIN_OUT, OUTPUT);
  pinMode(CLOCK_PIN_OUT, OUTPUT);
}

void loop() {

  for (int i = 0; i < 8; i++) {
    shiftOut(DATA_PIN_OUT, CLOCK_PIN_OUT, MSBFIRST, B00000001 << i);
    delay(DELAY);
  }

  for (int i = 0; i < 7; i++) {
    shiftOut(DATA_PIN_OUT, CLOCK_PIN_OUT, MSBFIRST, B1000000 >> i);
    delay(DELAY);
  }

}
