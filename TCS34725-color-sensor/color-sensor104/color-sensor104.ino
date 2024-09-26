// LED open (not grounded)

#include <Adafruit_TCS34725.h>
#include <Math.h>

const int INT_PIN = 2;
volatile boolean sampleAvailable = false;

uint16_t r;
uint16_t g;
uint16_t b;
uint16_t c;

const int NUM_SAMPLES = 5;
int samples[NUM_SAMPLES][4];
int sampleIdx = 0;

int NUM_COLORS = 6;
int COLORS_RGB[][3] = {
  { 13, 8, 9 },   // red
  { 6, 12, 6 },   // green
  { 3, 11, 13 },  // blue
  { 13, 7, 5 },   // orange
  { 6, 9, 10 },   // purple
  { 7, 10, 5 }    // yellow (last 9,13,6)
};
String COLORS_NAMES[] = {
  "red",
  "green",
  "blue",
  "orange",
  "purple",
  "yellow"
};

int getColorIdx(int r, int g, int b) {
  int lowestScore = 1000;
  int idx = -1;
  for (int c = 0; c < NUM_COLORS; c++) {
    int score = abs(r - COLORS_RGB[c][0]) + abs(g - COLORS_RGB[c][1]) + abs(b - COLORS_RGB[c][2]);
    // Serial.print("score:[") ; Serial.print(score); Serial.print("]("); Serial.print(c); Serial.println(")");
    if (score < 170 && score < lowestScore) {
      lowestScore = score;
      idx = c;
    }
  }
  return idx;
}

String getColorName(int r, int g, int b) {
  int idx = getColorIdx(r, g, b);
  return idx != -1 ? COLORS_NAMES[idx] : "unknown";
}

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);

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

  tcs.setIntLimits(0, 20); // clear light
  tcs.write8(TCS34725_PERS, TCS34725_PERS_1_CYCLE);
  tcs.setInterrupt(true);

  Serial.println("setup complete");
  Serial.flush();
}

/* tcs.getRawData() does a delay(Integration_Time) after the sensor readout.
We don't need to wait for the next integration cycle because we receive an interrupt when the integration cycle is complete*/
void getRawData_noDelay(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
  *c = tcs.read16(TCS34725_CDATAL);
  *r = tcs.read16(TCS34725_RDATAL);
  *g = tcs.read16(TCS34725_GDATAL);
  *b = tcs.read16(TCS34725_BDATAL);
}

int cT, rT, gT, bT;  // totals for averaging samples

void loop() {

  if (sampleAvailable) {
    getRawData_noDelay(&r, &g, &b, &c);
    Serial.println(getColorName(r, g, b));

    // dump values (c,r,g,b)
    // Serial.print(int(c));Serial.print(",");
    // Serial.print(int(r));Serial.print(",");
    // Serial.print(int(g));Serial.print(",");
    // Serial.print(int(b));
    // Serial.print("\n");

    // // BEGIN average samples
    // // capture sample
    // samples[sampleIdx][0] = c;
    // samples[sampleIdx][1] = r;
    // samples[sampleIdx][2] = g;
    // samples[sampleIdx][3] = b;
    // // update totals
    // cT += c;
    // rT += r;
    // gT += g;
    // bT += b;
    // // dump when num samples have been collected and reset sampleIdx
    // sampleIdx++;
    // if (sampleIdx == NUM_SAMPLES) {
    //   for (int idx=0; idx<NUM_SAMPLES; idx++) {
    //     Serial.print("sample:\t");
    //     Serial.print(idx);
    //     for (int chnl=0; chnl<4; chnl++) {
    //       Serial.print("\t");
    //       Serial.print(samples[idx][chnl]);
    //     }
    //     Serial.println("");
    //   }
    //     Serial.println("");
    //     Serial.println("Totals / Average");
    //     Serial.print("C: "); Serial.print(cT); Serial.print(" "); Serial.println(cT/NUM_SAMPLES);
    //     Serial.print("R: "); Serial.print(rT); Serial.print(" "); Serial.println(rT/NUM_SAMPLES);
    //     Serial.print("G: "); Serial.print(gT); Serial.print(" "); Serial.println(gT/NUM_SAMPLES);
    //     Serial.print("B: "); Serial.print(bT); Serial.print(" "); Serial.println(bT/NUM_SAMPLES);
    //     Serial.println();
    //   // reset
    //   sampleIdx = 0;
    //   cT=0;
    //   rT=0;
    //   gT=0;
    //   bT=0;
    // }
    // // END average samples

    sampleAvailable = false;
    delay(300);
    tcs.clearInterrupt();
  }
}

void isr() {
  sampleAvailable = true;
}
