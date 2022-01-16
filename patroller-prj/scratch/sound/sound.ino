

#include <SoftwareSerial.h>
#include "pitches.h"

int speakerPin = 11;

const int duration = 200;

int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A

// notes to play, corresponding to the 3 sensors:
int notes[] = {
  NOTE_B0, NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1, NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2, NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3, NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5, NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6, NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7, NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8
};

void setup()
{

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //  for (int i = 0; i < numTones; i++)
  //  {
  //    tone(speakerPin, tones[i]);
  //    delay(100);
  //  }
  //  noTone(speakerPin);
  //
  //  delay(1000);

  //  for (int i = 0; i < 89; i++)
  //  {
  //    tone(speakerPin, notes[i]);
  //    delay(100);
  //  }
  //  noTone(speakerPin);



}

void loop()
{
        sndDrive(true);
        delay(3000);
        sndDrive(false);

  sndFire();
  sndFire();
  sndFire();

  sndDone();

  delay(1000 * 60 * 60 * 5);
}

void sndDrive(boolean on)
{
  if (on)
  {
    tone(speakerPin, NOTE_D1);
  }
  else
  {
    noTone(speakerPin);
  }
}

void sndDone()
{
  tone(speakerPin, NOTE_C4, 80);
  delay(200);
  tone(speakerPin, NOTE_C6, 80);
  delay(200);
  tone(speakerPin, NOTE_D4, 80);
  delay(200);
  tone(speakerPin, NOTE_D6, 80);
  delay(200);
}

void sndFire()
{
  for (int i = 80; i < 1000; i += 20)
  {
    tone(speakerPin, i, 4);
    delay(6);
  }
  noTone(speakerPin);
}
