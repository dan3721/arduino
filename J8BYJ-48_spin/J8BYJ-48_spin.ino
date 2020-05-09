
/*
  This program spins the 28BYJ-48 stepper motor round and round.

  Created 5/8/2020
*/

#include <Stepper.h>

const int STEPS_PER_REV = 2038;
const int RPM = 3.8;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(STEPS_PER_REV, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(RPM);
  Serial.begin(9600);
}

void loop() {
  myStepper.step(STEPS_PER_REV);
  Serial.println("spinning!");
}
