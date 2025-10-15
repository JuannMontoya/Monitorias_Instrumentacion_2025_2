volatile unsigned long lastTime = 0;
volatile unsigned long currentTime = 0;
volatile unsigned long interval = 0;
volatile bool newData = false;

const int buttonPin = 2;  // Pin con capacidad de interrupción (GPIO2)

void IRAM_ATTR handleInterrupt() {
  currentTime = micros();         // Tiempo actual
  interval = currentTime - lastTime;
  lastTime = currentTime;
  newData = true;                 // Señal al loop() de que hay nuevo dato
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLDOWN);  // Configura pin con resistencia interna pulldown

  // Asocia interrupción al pin
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleInterrupt, RISING);

  Serial.println("=== ESP32C6: Medidor de tiempo entre pulsos ===");
  Serial.println("Presiona el botón para generar interrupciones...");
  Serial.println();
}

void loop() {
  if (newData) {
    noInterrupts();                // Evita conflicto de lectura
    unsigned long localInterval = interval;
    newData = false;
    interrupts();

    Serial.print("Tiempo entre pulsos: ");
    Serial.print(localInterval);
    Serial.println(" us");

    // Calcular frecuencia aproximada
    if (localInterval > 0) {
      float freq = 1000000.0 / localInterval;
      Serial.print("Frecuencia estimada: ");
      Serial.print(freq);
      Serial.println(" Hz");
    }

    Serial.println();
  }
}
