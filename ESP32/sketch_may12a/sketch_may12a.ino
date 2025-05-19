//librerias//
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>

//configuracion internet
const char* ssid = "FAMILIAHUELGOS";
const char* password = "3156849165";

WebServer server(80);
  
//Pines y configuracion
#define PIN_TEMP 4  //sensor de temperatura
#define PIN_PH A0   //sensor de ph
#define PIN_LED 2   //luz uv

//libreria onewire para el sensor de temperatura
OneWire oneWire (PIN_TEMP);
DallasTemperature sensors(&oneWire);

void setup(){
  Serial.begin(115200);  
  sensors.begin();
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);  //establece luz uv apagado por defecto

  //WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado al WiFi");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

 /* server.on("/", handleRoot);
  server.begin();*/

  Serial.println("Sistema de purificacion iniciado..."); 
}

/*void loop(){
  server.handleClient();
}

void handleRoot() {
  float temperatura = 24.5;
  float ph = 7.2;
  String html = "<!DOCTYPE html><html><head><title>Lecturas</title></head><body>";
  html += "<h1>Purificador de agua</h1>";
  html += "<p>temperatura: "+String(temperatura) + "grados</p>";
  html += "<p>ph: "+String(ph) + "</p>";
  html += "</body></html>"; 
  server.send(200, "text/html", "<h1>ESP32 en linea</h1><p>Funciona Bien</p>");
}*/


void loop(){

  //obtener temperatura
  float temperatura = random(1800, 3200) /100.0;
  //con sensor
//sensors.requestTemperatures();
//float Temperatura= sensors.getTempCByIndex(0);

  //obtener ph
  float ph = random(550, 850) /100.0;

  //evaluar calidad de agua
  bool aguaSegura = ( ph >= 6.5 && ph <= 8.5) && (temperatura >= 18.0 && temperatura <= 30.0);

  //mostrar en panel serial
  Serial.print("PH: ");
  Serial.print(ph);
  Serial.print(" temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  //evaluacion de calidad de agua
  if(aguaSegura){
    Serial.println("ESTADO DEL AGUA: SEGURA");
    Serial.println("(luz uv apagada)");
  }else{
    Serial.println("ESTADO DEL AGUA: NO SEGURA ---- ACTIVANDO PURIFICACION");
    Serial.println("(luz uv encendida)");
  }

  Serial.println("------------------------");
  delay(100000); //espera de 5 segundos
}

