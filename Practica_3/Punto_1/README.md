# PUNTO 1 UART: Juego de adivinar el número por comunicación serial USB (ESP32)

## Descripción general

En este proyecto se desarrolla un **juego interactivo** en el que el usuario debe **adivinar un número secreto** generado aleatoriamente por el **ESP32**.  
Toda la comunicación se realiza a través del **puerto USB (Serial)** mediante el monitor serial del **Arduino IDE**.  

El programa solicita primero al jugador establecer un **rango de números** (mínimo y máximo).  
Luego, el usuario envía sus **intentos por el monitor serial**, y el ESP32 responde si el número secreto es **mayor o menor**, hasta que se acierte.  
Finalmente, muestra la **cantidad de intentos realizados** y permite reiniciar el juego con un nuevo rango.

---

## Componentes requeridos

| Componente | Cantidad | Descripción |
|-------------|-----------|-------------|
| ESP32 DevKit | 1 | Microcontrolador principal |
| Cable USB | 1 | Para conexión y comunicación serial |
| PC con Arduino IDE | 1 | Para enviar y recibir datos seriales |

---

## Esquema de conexión

No se requiere conexión externa adicional:  
todo el intercambio de información se realiza por el **puerto USB** que utiliza el **Serial principal del ESP32**.

```
ESP32 <====USB====> PC / Monitor Serial (Arduino IDE)
```

---

## 🧠 Lógica del programa

1. Al iniciar, el ESP32 pide al usuario definir el **rango de números** (mínimo y máximo).  
2. El ESP32 genera un número aleatorio dentro de ese rango.  
3. El jugador envía intentos por el monitor serial.  
4. El ESP32 responde si el número secreto es **mayor, menor o correcto**.  
5. Se cuenta el número de intentos realizados.  
6. Una vez acertado, el juego se reinicia solicitando un nuevo rango.

---

## Código del proyecto

```cpp
// ===============================================================
//  Juego de Adivinar el Número por UART (Serial USB) - ESP32
// ===============================================================
//  Autor: Juan Esteban
//  Curso: Intrumentacion electronica
// ===============================================================

int numeroSecreto;     // número aleatorio que el jugador debe adivinar
int intento;           // valor ingresado por el usuario
int intentos = 0;      // contador de intentos
int rangoMin = 1;      // límite inferior del rango
int rangoMax = 100;    // límite superior del rango
bool juegoIniciado = false;  // indica si el juego ya comenzó
bool adivinado = false;      // indica si el número fue adivinado

void setup() {
  Serial.begin(115200);           // inicializa la comunicación por USB
  randomSeed(analogRead(0));      // semilla aleatoria usando ruido analógico
  Serial.println("=== Juego de Adivinar el Número ===");
  Serial.println("Envía dos números separados por espacio para definir el rango (Ejemplo: 10 50)");
}

void loop() {
  // --- Configuración del rango antes de iniciar ---
  if (!juegoIniciado && Serial.available() > 0) {
    rangoMin = Serial.parseInt();     // lee el primer número (mínimo)
    rangoMax = Serial.parseInt();     // lee el segundo número (máximo)
    if (rangoMin < rangoMax && rangoMax > 0) {
      numeroSecreto = random(rangoMin, rangoMax + 1);  // genera el número aleatorio
      juegoIniciado = true;
      intentos = 0;
      Serial.print("Juego iniciado. Adivina el número entre ");
      Serial.print(rangoMin);
      Serial.print(" y ");
      Serial.println(rangoMax);
    }
  }

  // --- Lógica del juego ---
  if (juegoIniciado && Serial.available() > 0) {
    intento = Serial.parseInt();    // lee el intento del jugador
    if (intento >= rangoMin && intento <= rangoMax) {
      intentos++;                   // incrementa el contador de intentos
      if (intento == numeroSecreto) {
        Serial.print("¡Correcto! Adivinaste el número en ");
        Serial.print(intentos);
        Serial.println(" intentos.");
        adivinado = true;
      } else if (intento < numeroSecreto) {
        Serial.println("El número es mayor.");
      } else {
        Serial.println("El número es menor.");
      }
    }
  }

  // --- Reinicio del juego ---
  if (adivinado) {
    delay(2000);
    Serial.println("Envía nuevo rango para jugar otra vez (Ejemplo: 10 100)");
    juegoIniciado = false;
    adivinado = false;
  }
}
```

---

## 💡 Ajustes recomendados

- Se puede modificar la velocidad del serial (`115200`) según preferencia.  
- Para mantener el juego activo, asegurarse de que el **monitor serial** esté configurado en **“Ambos NL & CR”** y **115200 baudios**.  
- Puede añadirse una versión con **tiempo límite**, **niveles de dificultad** o **puntuación** para gamificar la práctica.

---

## 🎯 Resultados esperados

Por el monitor serial se observará una interacción como la siguiente:

```
=== Juego de Adivinar el Número ===
Envía dos números separados por espacio para definir el rango (Ejemplo: 10 50)
10 30
Juego iniciado. Adivina el número entre 10 y 30
15
El número es mayor.
25
El número es menor.
20
¡Correcto! Adivinaste el número en 3 intentos.
Envía nuevo rango para jugar otra vez (Ejemplo: 10 100)
```

---

## 👨‍💻 Autor

**Juan Esteban**  
Proyecto educativo de práctica de comunicación serial en **ESP32**,  
desarrollado para el curso de **Instrumentación Electrónica / Sistemas Embebidos**.
