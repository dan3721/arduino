#include <SerialWombat.h>

SerialWombat sw;
SerialWombatPWM PWM1(sw);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Wire.begin();
  sw.begin(Wire, 0x6C);
  if (sw.queryVersion()) {
    Serial.println("wombat found");
  } else {
    Serial.println("wombat not found");
  }
  Serial.println("setup complete");

  // https://broadwellconsultinginc.github.io/SerialWombatArdLib/_serial_wombat_p_w_m_8h.html#a2088b99b4668da0234cd4564f5a328ba
  PWM1.setFrequency_SW4AB(SW4AB_PWMFrequency_488_Hz);
  // PWM1.setFrequency_SW4AB(SW4AB_PWMFrequency_16_Hz);

  // dutyCycle	A value from 0 to 65535 representing duty cycle
  // PWM1.begin(1, 65535/2); // 50%
  PWM1.begin(1, 65535*.357); 
}


void loop() {
  delay(1000);
  Serial.println(".");
}
