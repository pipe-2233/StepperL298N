/*
  BasicStepper - Ejemplo básico de la librería StepperL298N
  
  Este ejemplo muestra cómo usar la librería StepperL298N para controlar
  un motor paso a paso conectado a un driver L298N.
  
  ⚠️ ADVERTENCIAS IMPORTANTES:
  - El módulo L298N se CALIENTA mucho - usa disipador de calor
  - El motor paso a paso también se calienta - no tocar durante funcionamiento
  - Permite períodos de enfriamiento entre usos prolongados
  - Usa fuente de alimentación externa para el L298N (no desde Arduino)
  
  Conexiones:
  - Pin 32 -> IN1 del L298N
  - Pin 33 -> IN2 del L298N  
  - Pin 25 -> IN3 del L298N
  - Pin 26 -> IN4 del L298N
  
  Creado por Andre
  Este código es de dominio público.
*/

#include <StepperL298N.h>

// Crear instancia del motor paso a paso
// StepperL298N(pasos_por_revolución, pin1, pin2, pin3, pin4)
StepperL298N myStepper(200, 32, 33, 25, 26);

void setup() {
  Serial.begin(115200);
  Serial.println("Ejemplo básico - StepperL298N");
  Serial.println("=============================");
  Serial.println("⚠️  ADVERTENCIAS DE SEGURIDAD:");
  Serial.println("   - L298N se calienta mucho - usa disipador");
  Serial.println("   - Motor se calienta - no tocar durante uso");
  Serial.println("   - Permite enfriamiento entre sesiones");
  Serial.println("=============================");
  
  // Configurar velocidad del motor (RPM)
  myStepper.setSpeed(2.0);
  
  Serial.println("Motor configurado a 2 RPM");
  Serial.println("Iniciando secuencia de movimientos...");
}

void loop() {
  Serial.println("Girando 100 pasos en sentido horario...");
  myStepper.step(100);
  delay(1000);
  
  Serial.println("Girando 100 pasos en sentido antihorario...");
  myStepper.step(-100);
  delay(1000);
  
  Serial.println("Pausa de 2 segundos...");
  delay(2000);
}
