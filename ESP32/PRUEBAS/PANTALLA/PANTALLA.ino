#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Pines conectados a la pantalla
#define TFT_CS     -1    // No se usa si está soldado
#define TFT_RST     4    // Reset
#define TFT_DC      2    // Data/Command
#define TFT_SCLK   18    // SPI Clock (SCL)
#define TFT_MOSI   23    // SPI MOSI (SDA)

// Inicializar el objeto pantalla
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  tft.initR(INITR_MINI160x80);  // Tipo de pantalla (según tamaño)
  tft.setRotation(1);          // Rotación horizontal
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Hola!");

  tft.setTextSize(1);
  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_CYAN);
  tft.print("Pantalla TFT OK");
}

void loop() {
  // No se necesita hacer nada aquí por ahora
}


