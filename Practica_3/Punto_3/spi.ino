#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ====== CONFIGURACIÓN BMP280 (SPI) ======
#define BMP_CS   5
#define BMP_SCK  18
#define BMP_MISO 19
#define BMP_MOSI 23

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);
#define SEALEVELPRESSURE_HPA (1013.25)

// ====== CONFIGURACIÓN OLED ======
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // Cambiar a 0x3D si no funciona
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // ===== Comunicación Serial =====
  Serial.begin(115200);
  Serial.println("=====================================");
  Serial.println("🔧 Iniciando práctica BMP280 (SPI) + OLED SSD1306");
  Serial.println("=====================================");

  // ===== Inicialización OLED =====
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("❌ Error: No se detecta la pantalla OLED.");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("BMP280 SPI + OLED");
  display.display();
  delay(1500);

  // ===== Inicialización BMP280 =====
  if (!bmp.begin()) {
    Serial.println("❌ Error: No se detecta el BMP280 (SPI). Verifica conexiones.");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("BMP280 no detectado");
    display.display();
    while (true);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,   // Temperatura sobremuestreo x2
                  Adafruit_BMP280::SAMPLING_X16,  // Presión sobremuestreo x16
                  Adafruit_BMP280::FILTER_X16,    // Filtro interno
                  Adafruit_BMP280::STANDBY_MS_500);

  Serial.println("✅ Sensor BMP280 (SPI) iniciado correctamente.\n");
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("BMP280 listo!");
  display.display();
  delay(1000);
}

void loop() {
  // ===== Lectura de datos =====
  float temperature = bmp.readTemperature();       // °C
  float pressure = bmp.readPressure() / 100.0F;    // hPa
  float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA); // m

  // ===== Mostrar por Serial =====
  Serial.println("===== LECTURA DEL BMP280 (SPI) =====");
  Serial.print("🌡️ Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("⛽ Presión: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("⛰️ Altitud: ");
  Serial.print(altitude);
  Serial.println(" m");
  Serial.println("==============================\n");

  // ===== Mostrar en OLED =====
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("BMP280 SPI");

  display.setTextSize(1);
  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Pres: ");
  display.print(pressure, 1);
  display.println(" hPa");

  display.setCursor(0, 48);
  display.print("Alt:  ");
  display.print(altitude, 1);
  display.println(" m");

  display.display();

  delay(2000);
}