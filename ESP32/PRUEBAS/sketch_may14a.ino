#include <OneWire.h>
#include <DallasTemperature.h>

// PIN donde conectaste el cable DATA del sensor (recomendado GPIO 4)
#define PIN_TEMP 2

// Crear objetos para la comunicación con el sensor
OneWire oneWire(PIN_TEMP);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  delay(100);
  Serial.println("Prueba de sensor DS18B20 iniciada...");
}

void loop() {
  sensors.requestTemperatures();  // Solicita lectura de temperatura
  float tempC = sensors.getTempCByIndex(0);  // Lee la temperatura en °C

  // Verifica si hay error (-127 indica fallo)
  if (tempC == -127.00) {
    Serial.println("Error: sensor no detectado o mal conectado");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" °C");
  }

  delay(3000);  // Espera 3 segundos entre lecturas
}
