# 🧠 Proyecto: Medidor de tiempo entre pulsos con interrupciones en ESP32

## 🎯 Objetivo
Aprender a usar **interrupciones externas** en una placa ESP32 para medir el **tiempo entre eventos** (por ejemplo, pulsaciones de un botón o señales de un sensor).  
Se visualizan los resultados en el **Monitor Serial**.

---

## ⚙️ Materiales necesarios
- 1 placa **ESP32** (cualquier modelo, aquí se adapta al ESP32-C6).  
- 1 **botón pulsador**.  
- 1 **resistencia de 10 kΩ** (opcional si no usas `INPUT_PULLDOWN`).  
- Cables y protoboard.  

---

## 🔌 Conexiones
- Conecta el **botón** entre **GPIO2** y **3.3V**.  
- Si tu placa no tiene *pulldown interno*, agrega una **resistencia de 10 kΩ** entre **GPIO2** y **GND**.  

El pin **GPIO2** admite interrupciones externas sin problemas en la ESP32.

---

## 💻 Explicación del código

### 1. Variables globales
```cpp
volatile unsigned long lastTime = 0;
volatile unsigned long currentTime = 0;
volatile unsigned long interval = 0;
volatile bool newData = false;
```
Estas variables se declaran como **`volatile`** porque pueden cambiar dentro de una **rutina de interrupción (ISR)**.  
El compilador debe saber que pueden modificarse de forma asíncrona.

---

### 2. Configuración inicial
```cpp
pinMode(buttonPin, INPUT_PULLDOWN);
attachInterrupt(digitalPinToInterrupt(buttonPin), handleInterrupt, RISING);
```
- Se configura el pin con una resistencia *pulldown* interna.  
- Se vincula una interrupción externa al pin con la función `handleInterrupt()`.  
- El modo `RISING` significa que la ISR se ejecutará cuando el pin pase de 0V → 3.3V (al presionar el botón).

---

### 3. Rutina de interrupción (ISR)
```cpp
void IRAM_ATTR handleInterrupt() {
  currentTime = micros();
  interval = currentTime - lastTime;
  lastTime = currentTime;
  newData = true;
}
```
- `IRAM_ATTR` indica al compilador que esta función se debe ubicar en la **memoria rápida interna (IRAM)** del ESP32, optimizando la respuesta.  
- `micros()` devuelve el tiempo actual en microsegundos desde que se inició el programa.  
- Se calcula el tiempo entre pulsos restando el instante anterior del actual.  
- Finalmente se marca `newData = true` para avisar al *loop()* que hay un nuevo dato disponible.

---

### 4. Bucle principal
```cpp
if (newData) {
  noInterrupts();
  unsigned long localInterval = interval;
  newData = false;
  interrupts();

  Serial.print("Tiempo entre pulsos: ");
  Serial.print(localInterval);
  Serial.println(" us");
}
```
- El *loop()* revisa si hay un nuevo dato disponible.  
- Se bloquean las interrupciones momentáneamente con `noInterrupts()` para leer la variable sin riesgo de interferencias.  
- Se imprime el intervalo en el **Monitor Serial**.  
- Luego se calcula y muestra la frecuencia:
  ```cpp
  float freq = 1000000.0 / localInterval;
  ```
  (porque 1 segundo = 1,000,000 microsegundos).

---

## 🧪 Prueba
1. Abre el **Monitor Serial** a 115200 baudios.  
2. Presiona el botón varias veces.  
3. Verás el tiempo entre pulsos y la frecuencia estimada (Hz).  

Ejemplo de salida:
```
Tiempo entre pulsos: 240123 us
Frecuencia estimada: 4.16 Hz
```

---

## ⚡ Qué se aprende
- Cómo usar interrupciones externas (`attachInterrupt`) en ESP32.  
- Cómo proteger variables compartidas entre ISR y *loop()* con `volatile` y `noInterrupts()`.  
- Cómo medir el tiempo entre eventos con `micros()`.  
- Cómo visualizar resultados en el **Monitor Serial**.

---

## 🧩 Extensiones opcionales
- Sustituye el botón por un **sensor de proximidad**, **encoder** o **sensor Hall** para medir velocidad o RPM.  
- Agrega un **LED** que parpadee si la frecuencia supera cierto umbral.  
- Usa el **Serial Plotter** para graficar el tiempo o frecuencia en tiempo real.

---

## 🚀 Conceptos clave de interrupciones en ESP32
- Las interrupciones permiten **responder inmediatamente** a eventos de hardware.  
- No se deben usar funciones que requieran mucho tiempo dentro de una ISR (como `Serial.print()` o `delay()`).  
- El atributo `IRAM_ATTR` es específico del ESP32 para asegurar una ejecución rápida.  
- Las funciones `micros()` y `millis()` son herramientas básicas para medición temporal precisa.

---
