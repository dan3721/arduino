/*
   5/1/2020
   
   Using interrupt to make the ultra sonic non-blocking.
   
   https://homediyelectronics.com/projects/arduino/arduinoprogramminghcsr04withinterrupts/?p=4
*/

const int ECHO = 3;
const int TRIGGER = 2;
const int DELAY = 1000; // millis

void setup() {
  pinMode(TRIGGER, OUTPUT); // green
  pinMode(ECHO, INPUT); // yellow
  attachInterrupt(digitalPinToInterrupt(ECHO), isr_echo, HIGH) ;
  Serial.begin(9600);
}

volatile int distance = 500;

bool running;
void loop() {
  trigger();
  //    Serial.println(distance);
  running = distance > 3;
  Serial.println(running ? "running" : "stopped");
  delay(100);
}

long last;
void trigger() {
  long now = millis();
  if (now > last + DELAY) {
    // trigger
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    last = now;
  }
}

//volatile long pulseStart;
void isr_echo() {
  static long pulseStart;
  switch (digitalRead(ECHO))  {
    case HIGH:
      pulseStart = micros();
      break;
    case LOW:
      long duration = micros() - pulseStart;
      distance = microsecondsToInches(duration);
      break;
  }
}


long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
