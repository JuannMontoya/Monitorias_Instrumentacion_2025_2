 # Fase A: BLUETOOTH: Control RGB y transmisión de datos desde ESP32 
 
 ## Descripción general 
 
 En esta práctica se implementa una comunicación Bluetooth Serial (SPP) entre un ESP32 y un dispositivo móvil. 
 El objetivo es que los estudiantes comprendan cómo el ESP32 puede recibir comandos desde una app como "Serial Bluetooth Terminal" 
 para controlar un LED RGB, y enviar datos (como temperatura simulada) hacia el celular. 
 
 Se usa el módulo Bluetooth integrado del ESP32 y la técnica de PWM para controlar los colores del LED. 
 
 --- 
 
 ## Componentes requeridos 
 
 | Componente | Cantidad | Descripción | 
 |-------------|-----------|-------------| 
 | ESP32 DevKit | 1 | Microcontrolador principal con Bluetooth |
 | LED RGB (cátodo común) | 1 | LED de tres colores controlado por PWM |
 | Resistencias 220 Ω | 3 | Limitan la corriente de cada canal del LED |
 | Protoboard y cables | 1 | Conexión de componentes |
 | Cable USB | 1 | Programación y alimentación |
 | Smartphone con “Serial Bluetooth Terminal” | 1 | Comunicación Bluetooth |
 
 --- 
 
 ## Esquema de conexión 
 
 | LED RGB Pin | ESP32 Pin | 
 |--------------|------------| 
 | Rojo (R) | GPIO 25 | 
 | Verde (G) | GPIO 26 | 
 | Azul (B) | GPIO 27 | 
 | Cátodo común | GND | 
 
 --- 
 
 ## Lógica del programa 
 
 1. El ESP32 inicia el módulo Bluetooth Serial con el nombre “ESP32_RGB”. 
 2. El microcontrolador espera comandos enviados desde el celular: 
    - ON → Enciende el LED en color rojo. 
    - OFF → Apaga el LED. 
    - #RRGGBB → Cambia el color mediante un código hexadecimal (por ejemplo, #00FF80). 
 3. Cada segundo, el ESP32 envía al celular un valor simulado de temperatura (25 ± 3 °C). 
 
 --- 
 
 ## Código del proyecto 
 
 ```cpp 
 #include "BluetoothSerial.h" 
 
 BluetoothSerial SerialBT; 
 
 int rPin = 25; 
 int gPin = 26; 
 int bPin = 27; 
 
 void setup() { 
   Serial.begin(115200); 
   SerialBT.begin("ESP32_RGB");  // Nombre Bluetooth visible 
   Serial.println("Bluetooth listo. Conéctate con la app Serial Bluetooth Terminal."); 
 
   ledcSetup(0, 5000, 8); 
   ledcSetup(1, 5000, 8); 
   ledcSetup(2, 5000, 8); 
   ledcAttachPin(rPin, 0); 
   ledcAttachPin(gPin, 1); 
   ledcAttachPin(bPin, 2); 
 } 
 
 void setColor(int r, int g, int b) { 
   ledcWrite(0, r); 
   ledcWrite(1, g); 
   ledcWrite(2, b); 
 } 
 
 void loop() { 
   if (SerialBT.available()) { 
     String msg = SerialBT.readStringUntil('\n'); 
     msg.trim(); 
 
     if (msg == "ON") setColor(255, 0, 0);      // Rojo 
     else if (msg == "OFF") setColor(0, 0, 0);  // Apagar 
     else if (msg.startsWith("#")) { 
       long color = strtol(msg.substring(1).c_str(), NULL, 16); 
       int r = (color >> 16) & 0xFF; 
       int g = (color >> 8) & 0xFF; 
       int b = color & 0xFF; 
       setColor(r, g, b); 
     } 
   } 
 
   float temp = 25 + random(-3, 3); 
   SerialBT.println(String("Temp: ") + temp + " °C"); 
   delay(1000); 
 } 
 ``` 
 
 --- 
 
 ## Instrucciones para el estudiante 
 
 1. Carga el código en tu ESP32 y abre el Serial Monitor. 
 2. En tu celular, instala la app “Serial Bluetooth Terminal”. 
 3. Activa el Bluetooth y empareja el dispositivo “ESP32_RGB”. 
 4. Conéctate desde la app y envía los comandos: 
    - ON → Enciende el LED rojo. 
    - OFF → Apaga el LED. 
    - #0000FF → Cambia el color a azul, por ejemplo. 
 5. Observa en el celular cómo el ESP32 envía datos de temperatura cada segundo. 
 
 --- 
 
 ## Resultados esperados 
 
 En la app del celular se recibirán mensajes como:

```bash
 Temp: 26 °C 
 Temp: 24 °C 
 Temp: 27 °C 
  ```
 
 Y al enviar comandos, el LED cambiará de color según lo solicitado. 
 
 --- 
 
 ## Análisis del funcionamiento 
 
 - BluetoothSerial.h permite una comunicación SPP (Serial Port Profile), simulando un puerto serie inalámbrico. 
 - Los canales PWM (ledcWrite) generan señales analógicas simuladas para controlar brillo y mezcla de color. 
 - El código demuestra comunicación bidireccional: recepción de comandos y envío de datos. 
 
 --- 



 # Fase B: WIFI: Control de LED RGB mediante Servidor Web en ESP32 
 
 ## Descripción general 
 
 En esta práctica se implementa un **servidor web local** alojado dentro del ESP32. 
 El microcontrolador crea una interfaz web a la cual se puede acceder desde cualquier dispositivo conectado a la misma red WiFi. 
 Desde esta página web, el usuario puede seleccionar un color mediante un control tipo “color picker”, 
 y el ESP32 ajusta los valores PWM de los pines conectados a un LED RGB para reproducir dicho color. 
 
 El objetivo es que los estudiantes comprendan el funcionamiento de la comunicación **HTTP**, 
 el manejo de **peticiones GET** y la generación dinámica de páginas HTML desde un servidor embebido. 
 
 --- 
 
 ## Componentes requeridos 
 
 | Componente | Cantidad | Descripción | 
 |-------------|-----------|-------------| 
 | ESP32 DevKit | 1 | Microcontrolador principal con WiFi integrado |
 | LED RGB (cátodo común) | 1 | Dispositivo de salida controlado por PWM |
 | Resistencias 220 Ω | 3 | Limitan la corriente de cada color |
 | Protoboard y cables | 1 | Conexión de componentes |
 | Cable USB | 1 | Programación y alimentación |
 | PC o celular con navegador | 1 | Para acceder al servidor web del ESP32 |
 
 --- 
 
 ## Esquema de conexión 
 
 | LED RGB Pin | ESP32 Pin | 
 |--------------|------------| 
 | Rojo (R) | GPIO 25 | 
 | Verde (G) | GPIO 26 | 
 | Azul (B) | GPIO 27 | 
 | Cátodo común | GND | 
 
 --- 
 
 ## Lógica del programa 
 
 1. El ESP32 se conecta a una red WiFi existente como cliente (modo STA). 
 2. Una vez conectado, imprime en el monitor serial la IP asignada. 
 3. Inicia un servidor HTTP en el puerto 80. 
 4. Al ingresar la IP desde un navegador, se muestra una página con un selector de color. 
 5. Cada vez que el usuario selecciona un color, el navegador envía los valores RGB mediante una petición GET al ESP32. 
 6. El microcontrolador actualiza el color del LED en tiempo real usando PWM. 
 
 --- 
 
 ## Código del proyecto 
 
 ```cpp 
 #include <WiFi.h> 
 #include <WebServer.h> 
 
 #define RED_PIN   25 
 #define GREEN_PIN 26 
 #define BLUE_PIN  27 
 
 const char* ssid = "TuSSID"; 
 const char* password = "TuPassword"; 
 
 WebServer server(80); 
 
 const int freq = 5000; 
 const int resolution = 8; 
 const int chR = 0; 
 const int chG = 1; 
 const int chB = 2; 
 
 void setColor(uint8_t r, uint8_t g, uint8_t b) { 
   ledcWrite(chR, r); 
   ledcWrite(chG, g); 
   ledcWrite(chB, b); 
   Serial.printf("Color -> R:%d  G:%d  B:%d\n", r, g, b); 
 } 
 
 String htmlPage() { 
   return R"rawliteral( 
 <!DOCTYPE html> 
 <html lang="es"> 
 <head> 
 <meta charset="UTF-8"> 
 <meta name="viewport" content="width=device-width, initial-scale=1.0"> 
 <title>Control LED RGB ESP32 PWM</title> 
 <style> 
   body { background: radial-gradient(circle at top, #1a237e, #000); color: white; text-align: center; font-family: 'Segoe UI'; } 
   input[type='color'] { width: 160px; height: 160px; border: none; border-radius: 10px; cursor: pointer; } 
   button { margin-top: 20px; padding: 10px 25px; border: none; background: #00c853; color: white; font-size: 1.2em; border-radius: 8px; cursor: pointer; } 
   button:hover { background: #00a94f; } 
 </style> 
 </head> 
 <body> 
   <h1>Control LED RGB (PWM) - ESP32</h1> 
   <input type="color" id="colorInput" value="#00ff00"> 
   <div id="rgbValue">R:0 G:255 B:0</div> 
   <button onclick="sendColor()">Enviar color</button> 
   <footer>Desarrollado para prácticas IoT con ESP32 ⚡</footer> 
   <script> 
     function sendColor() { 
       const color = document.getElementById('colorInput').value; 
       const r = parseInt(color.substr(1,2),16); 
       const g = parseInt(color.substr(3,2),16); 
       const b = parseInt(color.substr(5,2),16); 
       document.getElementById('rgbValue').innerText = `R:${r} G:${g} B:${b}`; 
       fetch(`/set?r=${r}&g=${g}&b=${b}`); 
     } 
   </script> 
 </body> 
 </html> 
 )rawliteral"; 
 } 
 
 void handleRoot() { 
   server.send(200, "text/html", htmlPage()); 
 } 
 
 void handleSet() { 
   if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) { 
     int r = server.arg("r").toInt(); 
     int g = server.arg("g").toInt(); 
     int b = server.arg("b").toInt(); 
     setColor(r, g, b); 
     server.send(200, "text/plain", "OK"); 
   } else { 
     server.send(400, "text/plain", "Error: faltan parámetros"); 
   } 
 } 
 
 void setup() { 
   Serial.begin(115200); 
   ledcSetup(chR, freq, resolution); 
   ledcSetup(chG, freq, resolution); 
   ledcSetup(chB, freq, resolution); 
   ledcAttachPin(RED_PIN, chR); 
   ledcAttachPin(GREEN_PIN, chG); 
   ledcAttachPin(BLUE_PIN, chB); 
   setColor(0, 0, 0); 
   WiFi.mode(WIFI_STA); 
   WiFi.begin(ssid, password); 
   Serial.print("Conectando a WiFi"); 
   while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); } 
   Serial.println("\n✅ Conectado a WiFi"); 
   Serial.print("🌐 IP del servidor: "); 
   Serial.println(WiFi.localIP()); 
   server.on("/", handleRoot); 
   server.on("/set", handleSet); 
   server.begin(); 
   Serial.println("Servidor web iniciado 🚀"); 
 } 
 
 void loop() { 
   server.handleClient(); 
 } 
 ``` 
 
 --- 
 
 ## Instrucciones para el estudiante 
 
 1. Carga el código en tu ESP32 desde Arduino IDE. 
 2. Modifica las líneas con tu red WiFi y contraseña: 
    const char* ssid = "TuSSID"; 
    const char* password = "TuPassword"; 
 3. Abre el monitor serial para ver la IP asignada al ESP32. 
 4. Conéctate a la misma red WiFi desde tu PC o celular. 
 5. En un navegador, escribe la IP mostrada (por ejemplo, http://192.168.1.104). 
 6. Usa el selector de color y presiona “Enviar color” para cambiar el color del LED. 
 
 --- 
 
 ## Resultados esperados 
 
 - En el navegador, el usuario visualiza una interfaz moderna con selector de color. 
 - Cada vez que el color cambia, el LED RGB reproduce el tono seleccionado. 
 - En el monitor serial se muestra: 

```bash
   Color -> R:255  G:0  B:128 
   Color -> R:0  G:255  B:64 
```
 
 --- 
 
 ## Análisis del funcionamiento 
 
 - La librería WebServer.h permite crear un servidor HTTP ligero directamente en el ESP32. 
 - Cada petición GET enviada desde el navegador se maneja con las funciones handleRoot() y handleSet(). 
 - Los parámetros r, g y b se obtienen de la URL, se convierten a enteros y se aplican al PWM. 
 - Se demuestra comunicación **cliente-servidor** sobre protocolo **TCP/IP** mediante WiFi. 
 
 --- 



  
 # Fase C: COMUNICACIÓN MQTT BIDIRECCIONAL CON ESP32 
  

 🔹 OBJETIVO 
 Comprender el funcionamiento del protocolo MQTT como medio de comunicación en IoT, 
 enviando datos desde el ESP32 hacia un broker y recibiendo comandos desde el computador 
 para controlar el color de un LED RGB. 

 🔹 DESCRIPCIÓN 
 En esta práctica, el ESP32 se conectará a una red WiFi y al broker MQTT. 
 De forma periódica, enviará un valor de sensor simulado (por ejemplo, temperatura). 
 Además, recibirá mensajes MQTT con códigos RGB para cambiar el color del LED. 

  
 ## MATERIALES 
  
 - ESP32 o ESP32-C6 
 - LED RGB común o tira WS2812 (según versión anterior) 
 - Resistencias según sea necesario 
 - Conexión WiFi disponible 
 - Computador con Mosquitto instalado 
  

  
 ##  CÓDIGO DE EJEMPLO PARA EL ESP32 
  

 Este ejemplo utiliza el broker público “broker.hivemq.com” 
 pero también puedes usar “test.mosquitto.org” si lo prefieres. 

  
 📜 Código en Arduino IDE 
  
 ```cpp 

#include <WiFi.h>
#include <PubSubClient.h>

#define RED_PIN   25
#define GREEN_PIN 26
#define BLUE_PIN  27

const char* ssid = "TuSSID";
const char* password = "TuPassword";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int freq = 5000;
const int resolution = 8;
const int chR = 0;
const int chG = 1;
const int chB = 2;

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(chR, r);
  ledcWrite(chG, g);
  ledcWrite(chB, b);
  Serial.printf("Color cambiado -> R:%d G:%d B:%d\n", r, g, b);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en [");
  Serial.print(topic);
  Serial.print("]: ");
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.println(msg);

  if (String(topic) == "esp32/led") {
    int r, g, b;
    if (sscanf(msg.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
      setColor(r, g, b);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ✅");
      client.subscribe("esp32/led");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" -> Reintentando...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  ledcSetup(chR, freq, resolution);
  ledcSetup(chG, freq, resolution);
  ledcSetup(chB, freq, resolution);
  ledcAttachPin(RED_PIN, chR);
  ledcAttachPin(GREEN_PIN, chG);
  ledcAttachPin(BLUE_PIN, chB);
  setColor(0, 0, 0);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectada ✅");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int randomValue = random(20, 35);  // Temperatura simulada
  char msg[50];
  sprintf(msg, "%d", randomValue);
  client.publish("esp32/sensor", msg);

  delay(3000);
}
```


 ## EXPLICACIÓN DEL FUNCIONAMIENTO 

  1.  El ESP32 se conecta a la red WiFi definida en las variables ssid y password. 
  2.  Luego, establece conexión con el broker MQTT usando PubSubClient. 
  3. Cada 3 segundos, publica un valor aleatorio en el tópico “esp32/sensor”. 
  4. El ESP32 se suscribe al tópico “esp32/led”, de donde recibirá comandos. 
  5. Cuando llega un mensaje con formato “R,G,B” (por ejemplo: 255,0,128), 
     la función callback analiza los valores y cambia el color del LED. 


 ## PRUEBAS CON MOSQUITTO EN EL COMPUTADOR 


  1. Abre una terminal y suscríbete al tópico para recibir datos del ESP32 
 mosquitto_sub -h broker.hivemq.com -t esp32/sensor 

  2. En otra terminal, envía un comando para cambiar el color del LED
  ```bash
 mosquitto_pub -h broker.hivemq.com -t esp32/led -m "255,0,0" 
 (Esto encenderá el LED en rojo) 
  ```

 3. Puedes probar otros colores 

 ```bash
 mosquitto_pub -h broker.hivemq.com -t esp32/led -m "0,255,0" 
 mosquitto_pub -h broker.hivemq.com -t esp32/led -m "0,0,255" 
 ```

  4. Observa en el monitor serial del Arduino IDE cómo el ESP32 
 recibe los mensajes y muestra los valores RGB aplicados. 

 ## 👨‍💻 Autor 
 
 Juan Esteban 
 Práctica desarrollada para el curso de **Instrumentación Electrónica / Sistemas Embebidos** 