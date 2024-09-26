#include <Wire.h>
#include <Adafruit_TCS34725.h>

const int INT_PIN = 2;
volatile boolean status = false;

uint16_t r;
uint16_t g;
uint16_t b;
uint16_t c;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_60X);

void setup() {

  pinMode(INT_PIN, INPUT_PULLUP);  //TCS interrupt output is Active-LOW and Open-Drain
  attachInterrupt(digitalPinToInterrupt(INT_PIN), isr, FALLING);

  Serial.begin(9600);
  while (!Serial)
    ;

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;
  }

  // tcs.setIntLimits(0, 5); // TCS34725_INTEGRATIONTIME_24MS TCS34725_GAIN_1X (low resolution values)
  // tcs.setIntLimits(0, 20); // TCS34725_INTEGRATIONTIME_24MS TCS34725_GAIN_4X (works)
  tcs.setIntLimits(0, 250);  // TCS34725_INTEGRATIONTIME_24MS TCS34725_GAIN_60X (best)
  // tcs.setIntLimits(0, 300); // TCS34725_INTEGRATIONTIME_614MS TCS34725_GAIN_4X
  tcs.write8(TCS34725_PERS, TCS34725_PERS_1_CYCLE);

  // Set persistence filter to generate an interrupt for every RGB Cycle, regardless of the integration limits
  // tcs.write8(TCS34725_PERS, TCS34725_PERS_NONE);

  // delay(1000);


  Serial.println("setup complete");
  Serial.flush();

  tcs.setInterrupt(true);
}

/* tcs.getRawData() does a delay(Integration_Time) after the sensor readout.
We don't need to wait for the next integration cycle because we receive an interrupt when the integration cycle is complete*/
void getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
  *c = tcs.read16(TCS34725_CDATAL);
  *r = tcs.read16(TCS34725_RDATAL);
  *g = tcs.read16(TCS34725_GDATAL);
  *b = tcs.read16(TCS34725_BDATAL);
}

void loop() {


  getRawData_noDelay(&r, &g, &b, &c);
  Serial.println(c);

  if (status) {


    // getRawData_noDelay(&r, &g, &b, &c);
    // Serial.println(c);

    Serial.print("C: ");
    Serial.print(int(c));
    Serial.print("\tR: ");
    Serial.print(int(r));
    Serial.print("\tG: ");
    Serial.print(int(g));
    Serial.print("\tB: ");
    Serial.print(int(b));
    Serial.println();


    delay(3000);
    status = false;
    tcs.clearInterrupt();
  }

  // Serial.println(".");
  delay(10);
}

void isr() {
  // Serial.println("ISR");
  //getRawData_noDelay(&r, &g, &b, &c);
  // tcs.getRawData(&r, &g, &b, &c);
  // Serial.print("====>");
  // Serial.println(c);
  // status = false;
  // tcs.clearInterrupt();
  status = true;
  //   //  Serial.println("ISR(out)");
}
