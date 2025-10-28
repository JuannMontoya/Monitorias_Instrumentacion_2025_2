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
      // Permitir código hexadecimal #RRGGBB
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
