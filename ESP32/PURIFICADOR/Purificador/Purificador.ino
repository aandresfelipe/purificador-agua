//librerias
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

//configuración de internet
const char* ssid = "WUSTA";
const char* password = "USTA8600";

//libreria Web y definicion del puerto
WebServer server(80);

//Pines y configuracion
#define TFT_CS    -1
#define TFT_RST    4
#define TFT_DC     2
#define TFT_SCLK  18
#define TFT_MOSI  23
#define PIN_TEMP 0  //sensor de temperatura
#define PIN_PH 33   //sensor de ph
#define PIN_LED 15  //luz uv
//#define PIN_TURB 17 //sensor de turbidez
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//libreria onewire para el sensor de temperatura
OneWire  oneWire (PIN_TEMP);
DallasTemperature sensors(&oneWire);
  
// Variables globales para temperatura y pH (accesibles desde handleRoot)
float temperatura = 0.0; // Inicializar con un valor por defecto
float ph = 0.0;          // Inicializar con un valor por defecto
float turbidez = 0.0;    // Inicializar con un valor por defecto

//Setup (configuraciones)
void setup(){
  Serial.begin(115200);        //velocidad serial
  sensors.begin();             //iniciar pines
  pinMode(PIN_LED, OUTPUT);    //establece led como salida
  digitalWrite(PIN_LED, LOW);  //establece luz uv apagado por defecto

   // Pantalla TFT
  tft.initR(INITR_MINI160x80);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  //WiFi configuracion interna
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado al WiFi");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  //iniciacion de la pagina
  server.on("/", handleRoot);
  server.begin();

  //aviso de la iniciacion del sistema
  Serial.println("Sistema de purificacion iniciado..."); 
}


void handleRoot() {

  //configuracion de la web LOCAL del sistema
  String html = "<!DOCTYPE html><html><head><title>Lecturas</title>";
  html += "<style>";
  html += "body { font-family: sans-serif; }";
  html += "h1 { color: #0056b3; }";
  html += "p { font-size: 1.1em; }";
  html += ".segura { color: green; font-weight: bold; }";
  html += ".no-segura { color: red; font-weight: bold; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Purificador de agua</h1>";
  html += "<p>Temperatura: "+String(temperatura) + " °C</p>";  //Muestra la variable global (temperatura) 
  html += "<p>PH: "+String(ph) + "</p>";                       //Muestra la variable global (ph)

  //Mostrar el estado del agua en la pagina web
  bool aguaSegura = ( ph >= 6.5 && ph <= 8.5) && (temperatura >= 18.0 && temperatura <= 30.0);
  if(aguaSegura){
      html += "<p>Estado del agua: <span class=\"segura\">SEGURA</span></p>";
  } else {
      html += "<p>Estado del agua: <span class=\"no-segura\">NO SEGURA</span></p>";
  }
  html += "</body></html>";

  //Envio de la pagina desde la ip local del esp
  server.send(200, "text/html", html);
}

//repeticion del proceso de monitoreo y purificación
void loop(){

  //actualizacion de los datos en la pagina web
  server.handleClient();
  
  //obtener temperatura
  sensors.requestTemperatures();
  temperatura= sensors.getTempCByIndex(0);

  //obtener ph
  //PH UNICAMENTE
  int valorRaw =analogRead(PIN_PH);         //lee el valor que entrega la sonda
  float voltaje = valorRaw * (3.3/4095.0);  //lo combierte en voltaje 

  //ph = random(550, 850) /100.0;
  //codigo sin simular
  ph = 7 + ((2.5 - voltaje) / 0.18);

  //obtener turbidez
  //turbidez = random(0, 3001) /1000.0;
  //codigo sin simular
  //int lectura = analogRead(PIN_TURB);
  //turbidez = lectura * (3.3 /4095.0);

  //evaluar calidad de agua
  bool aguaSegura = ( ph >= 6.5 && ph <= 8.5) && (temperatura >= 18.0 && temperatura <= 30.0); //&& (turbidez < 1.5);

  //mostrar en panel serial
  Serial.print("PH: ");
  Serial.print(ph);
  Serial.print(" temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C");
  Serial.print(" valor leido: ");
  Serial.print(valorRaw);
  Serial.print(" °C");
  Serial.print("Voltaje: ");
  Serial.print(voltaje);
  Serial.println(" V");

  //evaluacion de calidad de agua
 tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_CYAN);
  tft.print("PH: "); tft.println(ph, 2);
  tft.print("Temp: "); tft.print(temperatura, 2); tft.println(" C");

  if (aguaSegura) {
    tft.setTextColor(ST77XX_BLUE);
    tft.println("AGUA SEGURA");
  } else {
    tft.setTextColor(ST77XX_YELLOW);
    tft.println("PURIFICANDO...");
    digitalWrite(PIN_LED, HIGH);
  }
  delay(5000); //espera de 5 segundos
}

