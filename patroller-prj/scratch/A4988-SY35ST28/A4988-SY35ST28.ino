
/**
   Remember to set the current limit on the A4988 stepper!
   Current rating: 500 mA per coil
   VREF=8*0.500*0.068=0.272V
*/
const int STEP_A = 8;
const int STEP_B = 10;
const int STEP_DELAY = 1; // millis (tested down to 500 μs via delayMicroseconds)

void setup() {
  pinMode(STEP_A, OUTPUT);
  pinMode(STEP_B, OUTPUT);
  digitalWrite(STEP_A, LOW);
  digitalWrite(STEP_B, LOW);
}

void loop() {
  digitalWrite(STEP_A, HIGH);
  digitalWrite(STEP_B, HIGH);
  delay(STEP_DELAY);
  
  digitalWrite(STEP_A, LOW);
  digitalWrite(STEP_B, LOW);
  delay(STEP_DELAY);
}
