/*
   Simple car that runs forward till something blocks it's path
   using a 28BYJ-48 stepper motor and an HCSR-04 ultrisonic sensor.

   5/9/2020
*/

#include <Stepper.h>

/**
   HCSR-04 ultrisonic sensor. The ULN2003 driver to Arduino pin setup:
   Trig -> 6
   Echo -> 7
*/
const int PIN_TRIGGER = 6;
const int PIN_ECHO = 7;

/* 28BYJ-48 stepper motor. The ULN2003 driver to Arduino pin setup:
    INT1 -> 8
    INT2 -> 9
    INT3 -> 10
    INT4 -> 11
*/
const int STEPS_PER_REV = 2038;
const int RPM = 3.9;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(STEPS_PER_REV, 8, 9, 10, 11);

void setup() {

  // setup ultrasonic sensor
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // setup stepper
  myStepper.setSpeed(RPM);
  //Serial.begin(9600);
}

void loop() {
  int distanceInInches = getDistanceInInches();
  if (distanceInInches > 3) {
    goForward();
  }
}

int getDistanceInInches() {

  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);
  //  Serial.println("ping sent");

  long duration = pulseIn(PIN_ECHO, HIGH);
  // convert the time into a distance
  int inches = microsecondsToInches(duration);
  //  cm = microsecondsToCentimeters(duration);
  //  Serial.print("inches: ");
  //  Serial.println(inches);
  return inches;
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

void goForward() {
  myStepper.step(100);
}
