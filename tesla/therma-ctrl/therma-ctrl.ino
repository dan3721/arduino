/**

*/
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int PUMP = 9;

const int THERMASISTOR_PINS [] = {14, 15, 16};
const int NUM_THERMASISTORS = sizeof(THERMASISTOR_PINS) / sizeof(int);
float TEMPRATURE_READINGS[NUM_THERMASISTORS];

const float R1 = 10000; // 10k
const float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

Servo pump;

/**
   Samples thermasistor, calculates, and stores temprature in temprature readings array.
*/
float getTemp(int i)
{
  float Vo = analogRead(THERMASISTOR_PINS[i]);
  float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  float logR2 = log(R2);
  float T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;
  T = (T * 9.0) / 5.0 + 32.0;
  return T;
}

/**
   Samples all the thermasistors.
*/
void sampleTempratures()
{
  for (int i = 0; i <= NUM_THERMASISTORS - 1; i++) {
    TEMPRATURE_READINGS[i] = getTemp(i);
  }
}

/**
   Dumps tempratures to the dispaly.
*/
void displayTempratures()
{
  for (int i = 0; i <= NUM_THERMASISTORS - 1; i++) {
    String s = "T";
    s += i;
    s += " ";
    //s += TEMPRATURE_READINGS[i];
    char outstr[5];
    dtostrf(TEMPRATURE_READINGS[i], 4, 1, outstr);
    s += outstr;
    s += " ";
    s+=char(223);
    s += "F";
    lcd.setCursor(0, i);
    lcd.print(s);
    //    Serial.print(".");
  }
}

void doPump()
{
  int val;
  float temp = TEMPRATURE_READINGS[0];
  if (temp > 80) {
    val = 180; // high
  }
  else if (temp > 75) {
    val = 180 * .75; // medium
  }
  else if (temp > 70) {
    val = 180 * .5; // low
  }
  else {
    val = 0; // off
  }
  //  Serial.println(val);
  pump.write(val);
}

void setup()
{

  pump.attach(PUMP);

  Serial.begin(9600);
  while (!Serial);
  Serial.println("READY");

  lcd.init();
  lcd.backlight();
  
}


void loop()
{
  sampleTempratures();
  displayTempratures();
  doPump();
  delay(500);
}
