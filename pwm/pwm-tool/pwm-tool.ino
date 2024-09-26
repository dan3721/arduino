#include <LibPrintf.h>

/*
  Hookup pin 3 to Oscilloscope and view/select prescaler via the 
  Serial Monitor(Select "No Line Ending").

 https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm
 */

const float DUTY_CYCLE_ON_PCNT = .1; // pinned at 10%

// defaults
long prescaler = 64;
String frequency = "976Hz";

void display() {
   printf("DUTY_CYCLE_ON_PCNT:[%.2f] prescaler:[%li] frequency:[%s] TCCR2B:[%03b]\n", 
    DUTY_CYCLE_ON_PCNT, prescaler, frequency.c_str(), TCCR2B);
}

void setup() {

  Serial.begin(115200);
  while(!Serial);
  Serial.println("serial connected");

  pinMode(3, OUTPUT);
  // pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 256;
  OCR2B = 256 * DUTY_CYCLE_ON_PCNT;

  Serial.println("setup complete");
  Serial.println("available prescalers for TCCR2B(pin 3) are 0(stopped),8(7.81KHz),32(1.95KHz),64(976Hz)[default],128(488Hz),256(244Hz),1024(61Hz).");
  display();
}

void loop() {
 if (Serial.available() > 0) {
  prescaler = Serial.parseInt();
   switch (prescaler) {
     case 0: TCCR2B = 0b000; frequency = "*** stopped ***"; break; // stopped
     case 1: TCCR2B = _BV(CS20); frequency = "62.5Hz"; break;
     case 8: TCCR2B = _BV(CS21); frequency = "7.81KHz"; break;
     case 32: TCCR2B = _BV(CS21) | _BV(CS20); frequency = "1.95KHz"; break;
     case 64: TCCR2B = _BV(CS22); frequency = "976Hz"; break;
     case 128: TCCR2B = _BV(CS22) | _BV(CS20); frequency = "488Hz"; break;
     case 256: TCCR2B = _BV(CS22) | _BV(CS21); frequency = "244Hz"; break;
     case 1024: TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20); frequency = "61Hz"; break;
     default: printf("Unsupported prescaler:[%i]\n", prescaler);
   }
  display();
 }
 delay(100);
}
