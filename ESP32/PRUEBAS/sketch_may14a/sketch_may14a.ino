#include <OneWire.h>
#include <DallasTemperature.h>

// Pin donde se conecta la sonda (señal)
#define ONE_WIRE_BUS 4  // Cambia el número si usas otro pin GPIO

// Instancias de OneWire y del sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  Serial.println("Sensor de temperatura DS18B20 iniciado");
}

void loop() {
  sensors.requestTemperatures(); // Solicita la temperatura
  float tempC = sensors.getTempCByIndex(0); // Lee la temperatura del primer sensor conectado

  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperatura: ");
    Serial.print(tempC);
    Serial.println(" °C");
  } else {
    Serial.println("Error: Sensor no conectado");
  }

  delay(2000); // Espera 2 segundos
}

