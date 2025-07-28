/*
  AdvancedControl - Ejemplo avanzado de la librería StepperL298N
  
  Este ejemplo muestra características avanzadas como:
  - Control de velocidad dinámico
  - Movimientos continuos
  - Control por comandos serie
  
  ⚠️ ADVERTENCIAS IMPORTANTES:
  - El módulo L298N se CALIENTA mucho - usa disipador de calor y ventilación
  - El motor paso a paso se calienta significativamente durante funcionamiento continuo
  - Monitorea la temperatura - si está muy caliente para tocar, detén inmediatamente
  - Este ejemplo usa movimiento continuo - vigila las temperaturas constantemente
  
  Conexiones:
  - Pin 32 -> IN1 del L298N
  - Pin 33 -> IN2 del L298N  
  - Pin 25 -> IN3 del L298N
  - Pin 26 -> IN4 del L298N
  
  Comandos disponibles:
  - 's' -> Detener/Iniciar motor
  - 'd' -> Cambiar dirección
  - '+' -> Aumentar velocidad
  - '-' -> Disminuir velocidad
  
  Creado por Andre
  Este código es de dominio público.
*/

#include <StepperL298N.h>

// Crear instancia del motor paso a paso
StepperL298N myStepper(200, 32, 33, 25, 26);

// Variables de control
bool motorRunning = true;
int direction = 1;  // 1 = horario, -1 = antihorario
float currentSpeed = 2.0;
unsigned long lastStepTime = 0;
int stepDelay = 10;  // delay entre pasos en ms

void setup() {
  Serial.begin(115200);
  Serial.println("=== Control Avanzado StepperL298N ===");
  Serial.println("⚠️  ADVERTENCIAS CRÍTICAS DE SEGURIDAD:");
  Serial.println("   🔥 L298N se CALIENTA mucho - usar disipador y ventilación");
  Serial.println("   🔥 Motor se calienta significativamente - monitorear temperatura");
  Serial.println("   ⚡ Movimiento continuo = más calor - vigilar constantemente");
  Serial.println("   🛑 Si muy caliente para tocar = DETENER INMEDIATAMENTE");
  Serial.println("=====================================");
  Serial.println("Comandos disponibles:");
  Serial.println("  's' - Detener/Iniciar motor");
  Serial.println("  'd' - Cambiar dirección");
  Serial.println("  '+' - Aumentar velocidad");
  Serial.println("  '-' - Disminuir velocidad");
  Serial.println("=====================================");
  
  // Configurar velocidad inicial
  myStepper.setSpeed(currentSpeed);
  
  Serial.println("Motor iniciado a " + String(currentSpeed) + " RPM");
  Serial.println("Dirección: " + String(direction > 0 ? "Horario" : "Antihorario"));
}

void loop() {
  unsigned long currentTime = millis();
  
  // Motor paso a paso no bloqueante
  if (motorRunning && (currentTime - lastStepTime >= stepDelay)) {
    myStepper.step(direction);
    lastStepTime = currentTime;
  }
  
  // Procesar comandos del puerto serie
  if (Serial.available()) {
    char command = Serial.read();
    processCommand(command);
  }
  
  // Mostrar estado cada 3 segundos
  static unsigned long lastStatusTime = 0;
  if (currentTime - lastStatusTime >= 3000) {
    showStatus();
    lastStatusTime = currentTime;
  }
}

void processCommand(char cmd) {
  switch (cmd) {
    case 's':
    case 'S':
      toggleMotor();
      break;
      
    case 'd':
    case 'D':
      changeDirection();
      break;
      
    case '+':
      increaseSpeed();
      break;
      
    case '-':
      decreaseSpeed();
      break;
      
    case 'h':
    case 'H':
      showHelp();
      break;
      
    default:
      Serial.println("Comando no reconocido. Envía 'h' para ayuda.");
      break;
  }
}

void toggleMotor() {
  motorRunning = !motorRunning;
  if (motorRunning) {
    Serial.println(">>> MOTOR INICIADO");
  } else {
    Serial.println(">>> MOTOR DETENIDO");
    myStepper.stopMotor();
  }
}

void changeDirection() {
  direction = -direction;
  Serial.println(">>> DIRECCIÓN CAMBIADA: " + String(direction > 0 ? "Horario" : "Antihorario"));
}

void increaseSpeed() {
  if (currentSpeed < 10.0) {
    currentSpeed += 0.5;
    myStepper.setSpeed(currentSpeed);
    Serial.println(">>> VELOCIDAD: " + String(currentSpeed) + " RPM");
  } else {
    Serial.println("Velocidad máxima alcanzada (10 RPM)");
  }
}

void decreaseSpeed() {
  if (currentSpeed > 0.5) {
    currentSpeed -= 0.5;
    myStepper.setSpeed(currentSpeed);
    Serial.println(">>> VELOCIDAD: " + String(currentSpeed) + " RPM");
  } else {
    Serial.println("Velocidad mínima alcanzada (0.5 RPM)");
  }
}

void showStatus() {
  Serial.print("Estado: ");
  Serial.print(motorRunning ? "FUNCIONANDO" : "DETENIDO");
  Serial.print(" | Dirección: ");
  Serial.print(direction > 0 ? "Horario" : "Antihorario");
  Serial.print(" | Velocidad: ");
  Serial.print(currentSpeed);
  Serial.println(" RPM");
}

void showHelp() {
  Serial.println("\n=== COMANDOS DISPONIBLES ===");
  Serial.println("s - Detener/Iniciar motor");
  Serial.println("d - Cambiar dirección");
  Serial.println("+ - Aumentar velocidad");
  Serial.println("- - Disminuir velocidad");
  Serial.println("h - Mostrar esta ayuda");
  Serial.println("============================\n");
}
