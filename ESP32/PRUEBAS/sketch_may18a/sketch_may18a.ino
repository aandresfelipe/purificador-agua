#define PH_PIN 33

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(PH_PIN);
  float volt = raw * (3.3 / 4095.0);
  float ph = 7 + ((2.5 - volt) / 0.18); // fórmula estimada

  Serial.print("Raw: "); Serial.print(raw);
  Serial.print(" | Voltaje: "); Serial.print(volt);
  Serial.print(" V | PH estimado: "); Serial.println(ph);

  delay(1000);
}

