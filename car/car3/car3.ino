/*
   5/12/2020

   Car 3 is complete with two motors(left working in reverse) and non-blocking ultra sonic.

   https://homediyelectronics.com/projects/arduino/arduinoprogramminghcsr04withinterrupts/?p=4
*/

#include <Stepper.h>

const int ECHO = 3;
const int TRIGGER = 2;
const int DELAY = 100; // millis
const int LIMIT = 3; // num inches

const int STEPS_PER_REV = 2038;
const int RPM = 6;
const int STEP = 1;

/* 28BYJ-48 stepper motor. The ULN2003 driver to Arduino pin setup:

    INT1 -> 4
    INT2 -> 6 *
    INT3 -> 5 *
    INT4 -> 7

  note*: reverse AND need to setSpeed to negative!

    INT1 -> 8
    INT2 -> 9
    INT3 -> 10
    INT4 -> 11
*/
Stepper leftStepper(STEPS_PER_REV, 4, 5, 6, 7);
Stepper rightStepper(STEPS_PER_REV, 8, 9, 10, 11);

void setup() {

  // setup ultrasonic sensor
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO), isr_echo, HIGH);

  // setup stepper
  leftStepper.setSpeed(RPM);
  rightStepper.setSpeed(RPM);

  Serial.begin(9600);
}

volatile int distance = 500;

bool running;
void loop() {
  trigger();
  if (distance > LIMIT) {
    leftStepper.step(STEP * -1);
    rightStepper.step(STEP);
  }

  //    Serial.println(distance);
  //  running = distance > 3;
  //  Serial.println(running ? "running" : "stopped");
  delay(1);
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
