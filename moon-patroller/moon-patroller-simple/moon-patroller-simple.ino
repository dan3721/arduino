/*
   5/17/2020

   The is a very simplistic program to drive the SMARS - 28BYJ-48 equped
   with an ultrasonic sensor. It simply drives forward till blocked and
   then turns left and attemts to proceed...

   https://homediyelectronics.com/projects/arduino/arduinoprogramminghcsr04withinterrupts/?p=4
*/

#include <Stepper.h>

const int ECHO = 3;
const int TRIGGER = 2;
const int DELAY = 100; // millis
const int LIMIT = 3; // num inches

const int STEPS_PER_REV = 2038;
const int RPM = 10;
const int STEP = 1;
const int BACK_DRIVE_TURN_STEPS = STEPS_PER_REV / 2;
/* 28BYJ-48 stepper motor. The ULN2003 driver to Arduino pin setup:

  RIGHT

    INT1 -> 4
    INT2 -> 6 *
    INT3 -> 5 *
    INT4 -> 7

  note*: reverse AND need to setSpeed to negative!

  LEFT

    INT1 -> 8
    INT2 -> 9
    INT3 -> 10
    INT4 -> 11
*/
Stepper rightStepper(STEPS_PER_REV, 4, 5, 6, 7);
Stepper leftStepper(STEPS_PER_REV, 8, 9, 10, 11);

volatile int distance;

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

bool blocked;
void loop() {
  trigger();
  //  Serial.println(distance);
  blocked = distance != 0 && distance < LIMIT;

  if (blocked) {
    leftStepper.step(BACK_DRIVE_TURN_STEPS * -1);
  }
  else {
    leftStepper.step(STEP);
    rightStepper.step(STEP * -1);
  }
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

void isr_echo() {
  static long pulseStart;
  switch (digitalRead(ECHO))  {
    case HIGH:
      pulseStart = micros();
      break;
    case LOW:
      long microseconds = micros() - pulseStart;
      distance = microseconds / 74 / 2; // inches (http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf)
      break;
  }
}
