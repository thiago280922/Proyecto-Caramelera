
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Definir pines
const int S0 = 2;
const int S1 = 3;
const int S2 = 4;
const int S3 = 5;
const int OUT = 6;  // Salida de la frecuencia del sensor
const int servoPin1 = 9;  // Primer servo
const int servoPin2 = 10; // Segundo servo

// Crear objetos para los servos
Servo servo1;
Servo servo2;

// Crear objeto para la pantalla LCD 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección del LCD (0x27 es común)

// Variables para leer el color
int red, green, blue;
int colorCount[5] = {0, 0, 0, 0, 0}; // Conteo de Rojo, Amarillo, Naranja, Verde, Azul

// Colores
enum Colors { Rojo = 0, Amarillo, Naranja, Verde, Azul, NoDetectado };

// Función para leer el color
void readColor() {
  // Leer el valor del color rojo
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(100);  // Esperar un poco para que el sensor estabilice
  red = pulseIn(OUT, HIGH);

  // Leer el valor del color verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(100);
  green = pulseIn(OUT, HIGH);

  // Leer el valor del color azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(100);
  blue = pulseIn(OUT, HIGH);
}

// Función para detectar el color basado en los valores de rojo, verde y azul
int detectColor(int red, int green, int blue) {
  if (red > 150 && green > 1400 && blue > 500) {
    return Rojo;  // Rojo
  }
  if (red > 300 && green > 1500 && green < 1600 && blue > 550) {
    return Naranja; // Naranja
  }
  if (red > 2000 && green > 300 && blue > 300) {
    return Amarillo; // Amarillo
  }
  if (red > 1150 && green > 550 && blue > 550 ) {
    return Verde; // Verde
  }
  if (red > 200 && green > 100 && blue < 100) {
    return Azul;  // Azul
  }

  return NoDetectado;  // No detectado
}

void setup() {
  // Iniciar comunicación serial
  Serial.begin(9600);

  // Inicializar la pantalla LCD
  lcd.init();
  lcd.setBacklight(1);  // Encender la luz de fondo del LCD
  lcd.clear();
 
  // Mostrar mensaje inicial
  //lcd.setCursor(0, 0);
  //lcd.print("Esperando color...");
 
  // Configurar los pines del sensor TCS3200
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Iniciar los servos
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  // Configurar el sensor TCS3200 para medir los colores
  // Configurar S0 y S1 para obtener la frecuencia máxima (escala de salida más alta)
  digitalWrite(S0, HIGH);  
  digitalWrite(S1, LOW);  

  // Filtros de color: Predeterminado para rojo (S2 y S3 en LOW)
  digitalWrite(S2, LOW);  // Filtro rojo
  digitalWrite(S3, LOW);  // Filtro rojo
}

void loop() {
  // Leer el valor de los colores (rojo, verde, azul)
  readColor();

  // Mostrar los valores de color en el monitor serial
  Serial.print("Rojo: ");
  Serial.print(red);
  Serial.print(" Verde: ");
  Serial.print(green);
  Serial.print(" Azul: ");
  Serial.println(blue);

  // Detectar el color basado en los valores de rojo, verde y azul
  int detectedColor = detectColor(red, green, blue);

  // Actualizar el conteo de colores
  if (detectedColor != NoDetectado) {
    colorCount[detectedColor]++;
  }

  // Mostrar el color detectado en la LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Color: ");
  switch(detectedColor) {
    case Rojo:
      lcd.print("Rojo");
      servo1.write(0);   // Gira el primer servo a 0 grados
      servo2.write(0);   // Gira el segundo servo a 0 grados
      break;
    case Amarillo:
      lcd.print("Amarillo");
      servo1.write(45);  // Gira el primer servo a 45 grados
      servo2.write(45);  // Gira el segundo servo a 45 grados
      break;
    case Naranja:
      lcd.print("Naranja");
      servo1.write(135); // Gira el primer servo a 135 grados
      servo2.write(135); // Gira el segundo servo a 135 grados
      break;
    case Verde:
      lcd.print("Verde");
      servo1.write(90);  // Gira el primer servo a 90 grados
      servo2.write(90);  // Gira el segundo servo a 90 grados
      break;
    case Azul:
      lcd.print("Azul");
      servo1.write(180); // Gira el primer servo a 180 grados
      servo2.write(180); // Gira el segundo servo a 180 grados
      break;
    case NoDetectado:
      lcd.print("No Detectado");
      break;
  }

  // Mostrar el conteo de colores en la pantalla LCD
  lcd.setCursor(0, 1);
  lcd.print("R: ");
  lcd.print(colorCount[Rojo]);
  lcd.print(" A: ");
  lcd.print(colorCount[Amarillo]);
  lcd.print(" N: ");
  lcd.print(colorCount[Naranja]);

  delay(2000);  // Esperar 2 segundos antes de tomar la siguiente lectura
}
