/*
   Simple car that runs forward till something blocks it's path
   using a 28BYJ-48 stepper motor and an HCSR-04 ultrisonic sensor.

     This version does not suffer from the blocking nature of 
     Stepper.step and a bit less glitchey thanks to AccelStepper.runSpeed.

   5/9/2020
*/

#include <AccelStepper.h>

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
//const int STEPS_PER_REV = 2038;
const float MAX_STEPS_PER_SECOND = 100;

// initialize the stepper library on pins 8 through 11:
AccelStepper stepper(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

void setup() {

  // setup ultrasonic sensor
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // setup stepper
  stepper.setMaxSpeed(MAX_STEPS_PER_SECOND);
//  stepper.setAcceleration(50.0);
  stepper.setSpeed(MAX_STEPS_PER_SECOND);

  //  Serial.begin(9600);
}

void loop() {
  int distanceInInches = getDistanceInInches();
  if (distanceInInches > 3) {
    stepper.runSpeed();
  }
  else {
    stepper.stop();
  }
}

long lastDistanceCheckTime = -1;
int lastDistanceInInches = 0;
const int DISTANCE_CHECK_PERIOD_IN_MILLIS = 1000;

int getDistanceInInches() {

  // throttle
  long now = millis();
  if (now > lastDistanceCheckTime + DISTANCE_CHECK_PERIOD_IN_MILLIS) {

    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(5);
    digitalWrite(PIN_TRIGGER, LOW);
    //  Serial.println("ping sent");

    long duration = pulseIn(PIN_ECHO, HIGH);
    // convert the time into a distance
    int inches = microsecondsToInches(duration);
    //  cm = microsecondsToCentimeters(duration);
    //    Serial.print("inches: ");
    //    Serial.println(inches);

    lastDistanceCheckTime = now;
    lastDistanceInInches = inches;
  }
  return lastDistanceInInches;
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
