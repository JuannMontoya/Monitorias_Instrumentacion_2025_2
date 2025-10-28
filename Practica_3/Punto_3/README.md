 
 # PRÁCTICA 4: SENSOR BMP280 (SPI) + PANTALLA OLED SSD1306 


 ## OBJETIVO ## 
 Implementar un código en ESP32 que permita obtener los valores de 
 presión barométrica, temperatura y altitud del sensor BMP280 mediante 
 comunicación SPI y visualizarlos tanto en el monitor serial como en 
 una pantalla OLED SSD1306. 

 ## MATERIALES ## 
 -  ESP32 DevKit 
 -  Sensor BMP280 (versión SPI, no I2C) 
 -  Pantalla OLED SSD1306 (I2C 128x64) 
 -  Jumpers macho-macho 
 -  Protoboard 
 -  Cables USB para conexión al PC 

 ## CONEXIONES ## 
  CONEXIONES SPI (ESP32 ↔ BMP280) 
 ---------------------------------- 
   BMP280 (SPI)   |   ESP32 
   ----------------------------- 
   VCC            →   3.3V 
   GND            →   GND 
   SCK (CLK)      →   GPIO 18 
   SDI (MOSI)     →   GPIO 23 
   SDO (MISO)     →   GPIO 19 
   CS (Chip Sel)  →   GPIO 5 

   CONEXIONES I2C (ESP32 ↔ OLED SSD1306) 
 ---------------------------------- 
   OLED SSD1306   |   ESP32 
   ----------------------------- 
   VCC            →   3.3V 
   GND            →   GND 
   SDA            →   GPIO 21 
   SCL            →   GPIO 22 

 ## CONFIGURACIÓN DEL ENTORNO ## 
 1.  Abre el Arduino IDE 
 2. Instala las siguientes librerías desde el gestor de librerías: 
     - Adafruit BMP280 Library 
     - Adafruit Unified Sensor 
     - Adafruit SSD1306 
     - Adafruit GFX Library 
 3. Selecciona la placa: "ESP32 Dev Module" 
 4.  Selecciona el puerto correspondiente a tu ESP32 
 5.  Copia y pega el código en el IDE y súbelo. 

 ## CÓDIGO PRINCIPAL ## 
 ```cpp

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

 ```

 ## DESCRIPCIÓN DEL FUNCIONAMIENTO ## 
 Una vez cargado el código, el ESP32: 
 1.  Inicializa la comunicación SPI con el BMP280 
 2.  Configura la pantalla OLED SSD1306 mediante I2C 
 3.  Lee los valores de temperatura, presión y altitud cada 2 segundos 
 4.  Muestra los datos simultáneamente: 
     - En el Monitor Serial del IDE 
     - En la pantalla OLED SSD1306 

 ## RESULTADOS ESPERADOS ## 
 En el monitor serial se visualizará algo similar a: 
 ---------------------------------------------- 
  ```BASH
 ==== LECTURA DEL BMP280 (SPI) ==== 
 🌡️ Temperatura: 25.3 °C 
 ⛽ Presión: 1009.7 hPa 
 ⛰️ Altitud: 23.5 m 
 ---------------------------------------------- 
  ```

 Y en la pantalla OLED se mostrará: 
  ```bash
  ----------------------------- 
  BMP280 SPI 
  Temp: 25.3 C 
  Pres: 1009.7 hPa 
  Alt:  23.5 m 
  ----------------------------- 
   ```

 ## ACTIVIDAD SUGERIDA ## 
 📍 A partir de este código base, propón una mejora: 
     - Registrar los datos de presión y temperatura en un archivo CSV en microSD 
     - Enviar los valores a un servidor MQTT o ThingSpeak 
     - Graficar los datos en tiempo real usando Python o MATLAB 

 ## PREGUNTAS DE ANÁLISIS ## 
 1. ¿Por qué es necesario configurar el pin CS al usar SPI? 
 2. ¿Cuál es la diferencia entre usar el BMP280 por SPI o I2C? 
 3. ¿Qué efecto tiene cambiar la presión de referencia SEALEVELPRESSURE_HPA? 
 4.  ¿Cómo se puede optimizar la frecuencia de actualización del sensor? 
 5. ¿Qué modificaciones serían necesarias si se usa un BME280 (con humedad)? 

 ## CONCLUSIÓN ## 
 El estudiante logra implementar una comunicación SPI efectiva entre el 
 ESP32 y el sensor BMP280, interpretando datos ambientales y desplegándolos 
 de forma local mediante una pantalla OLED e interfaz serial. 
