
/**
   Remember to set the current limit on the A4988 stepper!
   Current rating: 500 mA per coil
   VREF=8*0.500*0.068=0.272V

   TODO:
   - add periodic beckon sound
   - add dump cmd
   - add dedicated 3v regulator
   - hook up dump servo (6-7.4v)
   - add resistor to speaker
   - flush linking messages
   - auto power off/sleep
   - misc/out?
   - self distruct on abort?
*/

#include <SoftwareSerial.h>

const int ABORT = 2;
const int COMLINK_RX = 3;
const int COMLINK_TX = 4;
//const int COMLINK_ENABLE = 5; // not wroking; borking abort interrupt?
const int STEP_ENABLE = 6;
const int A_DIR = 7;
const int A_STEP = 8;
const int B_DIR = 9;
const int B_STEP = 10;
const int SPEAKER = 11;
const int CANNON = 12;

const int STEP_DELAY = 1; // millis (tested down to 500 μs via delayMicroseconds)

SoftwareSerial commlink(COMLINK_TX, COMLINK_RX); // TX, TR from esp8266


void setup() {

  attachInterrupt(digitalPinToInterrupt(ABORT), abort, RISING);

  // motor setup
  pinMode(A_STEP, OUTPUT);
  pinMode(B_STEP, OUTPUT);
  enableMotors(false);
  pinMode(A_DIR, OUTPUT); digitalWrite(A_DIR, LOW);
  pinMode(B_DIR, OUTPUT); digitalWrite(B_DIR, LOW);

  pinMode(CANNON, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {}

  // com link setup
  commlink.begin(115200);
  Serial.println("ON PATROL");
}

void abort()
{
  enableMotors(false);
  noTone(SPEAKER);
  Serial.println("*** ABORTED ***");
  Serial.flush();
  exit(0);
}

void loop() {

  // get next cmd and repeat
  while (commlink.available() > 0)
  {
    char cmd = commlink.read();
    //    Serial.print("cmd:[");
    //    Serial.print(cmd);
    //    Serial.println("]");
    if (cmd == '-')
    {
      String msg = commlink.readString();
      Serial.println(msg);
      break;
    }
    else {
      int i = commlink.parseInt();
      //      Serial.print("i:[");
      //      Serial.print(i);
      //      Serial.println("]");

      switch (cmd)
      {
        case 'f': // forward
        case 'b': // backward
          move(cmd, i);
          break;
        case 'r': // right
        case 'l': // left
          turn(cmd, i);
          break;
        case '*': fire(i); break; // fire
        case 'h' : hold(i); break; // hold
        case 'd' : sndDone();
      }
    }
  }

}

void enableMotors(bool enable)
{
  digitalWrite(STEP_ENABLE, enable ? LOW : HIGH);
}

void move(char direction, int steps)
{
  if (direction == 'b') {
    digitalWrite(A_DIR, HIGH);
    digitalWrite(B_DIR, HIGH);
  }
  enableMotors(true);
  sndDrive(true);
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(A_STEP, HIGH);
    digitalWrite(B_STEP, HIGH);
    delay(STEP_DELAY);
    digitalWrite(A_STEP, LOW);
    digitalWrite(B_STEP, LOW);
    delay(STEP_DELAY);
  }
  enableMotors(false);
  sndDrive(false);
  digitalWrite(A_DIR,  LOW);
  digitalWrite(B_DIR,  LOW);
}

void turn(char direction, int steps)
{
  digitalWrite(direction == 'r' ? A_DIR : B_DIR, HIGH);
  move('f', steps);
  digitalWrite(A_DIR,  LOW);
  digitalWrite(B_DIR,  LOW);
}

void fire(int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(CANNON, HIGH);
    sndFire();
    digitalWrite(CANNON, LOW);
    delay(80);
  }
}

void hold(int time)
{
  delay(time);
}

void sndDrive(boolean on)
{
  if (on)
  {
    tone(SPEAKER, 37); // NOTE_D1
  }
  else
  {
    noTone(SPEAKER);
  }
}

const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_C6 = 1047;
const int NOTE_D6 = 1175;

void sndDone()
{
  tone(SPEAKER, NOTE_C4, 80);
  delay(200);
  tone(SPEAKER, NOTE_C6, 80);
  delay(200);
  tone(SPEAKER, NOTE_D4, 80);
  delay(200);
  tone(SPEAKER, NOTE_D6, 80);
  delay(200);
}

void sndFire()
{
  for (int i = 80; i < 1000; i += 20)
  {
    tone(SPEAKER, i, 4);
    delay(6);
  }
  noTone(SPEAKER);
}
