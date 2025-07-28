/*
  SafeOperation - Ejemplo con monitoreo de seguridad
  
  Este ejemplo demuestra cómo implementar pausas automáticas para
  prevenir el sobrecalentamiento del L298N y motor paso a paso.
  
  ⚠️ CARACTERÍSTICAS DE SEGURIDAD:
  - Pausas automáticas cada 30 minutos
  - Monitoreo de tiempo de funcionamiento
  - Advertencias de temperatura en consola
  - Ciclo de trabajo controlado
  
  Conexiones:
  - Pin 32 -> IN1 del L298N
  - Pin 33 -> IN2 del L298N  
  - Pin 25 -> IN3 del L298N
  - Pin 26 -> IN4 del L298N
  
  RECOMENDADO: Agregar sensor de temperatura (TMP36, DS18B20)
  para monitoreo automático real.
  
  Creado por Andre
  Este código es de dominio público.
*/

#include <StepperL298N.h>

// Crear instancia del motor paso a paso
StepperL298N myStepper(200, 32, 33, 25, 26);

// Configuración de seguridad
const unsigned long TIEMPO_TRABAJO = 30 * 60 * 1000;    // 30 minutos en ms
const unsigned long TIEMPO_ENFRIAMIENTO = 10 * 60 * 1000; // 10 minutos en ms
const unsigned long ADVERTENCIA_CALOR = 20 * 60 * 1000;   // Advertir a los 20 min

// Variables de control de seguridad
unsigned long tiempoInicioTrabajo = 0;
unsigned long tiempoInicioEnfriamiento = 0;
bool estaEnfriando = false;
bool advertenciaEmitida = false;
bool sistemaActivo = true;

// Variables de control del motor
bool motorRunning = true;
int direction = 1;  // 1 = horario, -1 = antihorario
float currentSpeed = 1.0;  // Velocidad más baja para menor calor
unsigned long lastStepTime = 0;
int stepDelay = 20;  // delay más alto para menor calor

void setup() {
  Serial.begin(115200);
  Serial.println("=== Operación Segura StepperL298N ===");
  Serial.println("🔥 SISTEMA CON MONITOREO DE SEGURIDAD 🔥");
  Serial.println("=====================================");
  Serial.println("⚠️  Configuración de seguridad:");
  Serial.println("   - Trabajo: 30 min máximo continuo");
  Serial.println("   - Enfriamiento: 10 min obligatorio");
  Serial.println("   - Velocidad: Reducida para menor calor");
  Serial.println("   - Monitoreo: Automático");
  Serial.println("=====================================");
  
  // Configurar velocidad más baja para generar menos calor
  myStepper.setSpeed(currentSpeed);
  tiempoInicioTrabajo = millis();
  
  Serial.println("Sistema iniciado - Velocidad: " + String(currentSpeed) + " RPM");
  Serial.println("Tiempo de trabajo restante: 30 minutos");
  Serial.println("Comandos: 's'=parar, 'd'=dirección, 'i'=info");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Verificar estado de seguridad
  verificarSeguridad(currentTime);
  
  // Motor paso a paso no bloqueante (solo si no está enfriando)
  if (motorRunning && sistemaActivo && !estaEnfriando) {
    if (currentTime - lastStepTime >= stepDelay) {
      myStepper.step(direction);
      lastStepTime = currentTime;
    }
  }
  
  // Procesar comandos del puerto serie
  if (Serial.available()) {
    char command = Serial.read();
    processCommand(command);
  }
  
  // Mostrar estado cada 60 segundos
  static unsigned long lastStatusTime = 0;
  if (currentTime - lastStatusTime >= 60000) {
    mostrarEstadoSeguridad(currentTime);
    lastStatusTime = currentTime;
  }
}

void verificarSeguridad(unsigned long currentTime) {
  if (!estaEnfriando) {
    // Verificar tiempo de trabajo
    unsigned long tiempoTrabajando = currentTime - tiempoInicioTrabajo;
    
    // Advertencia a los 20 minutos
    if (tiempoTrabajando >= ADVERTENCIA_CALOR && !advertenciaEmitida) {
      Serial.println("\n🔥 ADVERTENCIA: 20 minutos de trabajo continuo");
      Serial.println("   Verificar temperatura del L298N y motor");
      Serial.println("   Pausa automática en 10 minutos\n");
      advertenciaEmitida = true;
    }
    
    // Pausa forzosa a los 30 minutos
    if (tiempoTrabajando >= TIEMPO_TRABAJO) {
      iniciarEnfriamiento(currentTime);
    }
  } else {
    // Verificar tiempo de enfriamiento
    unsigned long tiempoEnfriando = currentTime - tiempoInicioEnfriamiento;
    
    if (tiempoEnfriando >= TIEMPO_ENFRIAMIENTO) {
      finalizarEnfriamiento(currentTime);
    }
  }
}

void iniciarEnfriamiento(unsigned long currentTime) {
  estaEnfriando = true;
  motorRunning = false;
  myStepper.stopMotor();
  tiempoInicioEnfriamiento = currentTime;
  
  Serial.println("\n🛑 PAUSA OBLIGATORIA DE ENFRIAMIENTO INICIADA");
  Serial.println("   Duración: 10 minutos");
  Serial.println("   Verificar temperatura de componentes");
  Serial.println("   ¡NO tocar L298N ni motor hasta que enfríen!\n");
}

void finalizarEnfriamiento(unsigned long currentTime) {
  estaEnfriando = false;
  motorRunning = true;
  advertenciaEmitida = false;
  tiempoInicioTrabajo = currentTime;
  
  Serial.println("\n✅ ENFRIAMIENTO COMPLETADO");
  Serial.println("   Reanudando operación normal");
  Serial.println("   Nuevo ciclo de 30 minutos iniciado\n");
}

void mostrarEstadoSeguridad(unsigned long currentTime) {
  Serial.println("\n=== ESTADO DE SEGURIDAD ===");
  
  if (estaEnfriando) {
    unsigned long tiempoEnfriando = currentTime - tiempoInicioEnfriamiento;
    unsigned long tiempoRestante = TIEMPO_ENFRIAMIENTO - tiempoEnfriando;
    Serial.println("🔄 ENFRIANDO - Tiempo restante: " + String(tiempoRestante / 60000) + " min");
  } else {
    unsigned long tiempoTrabajando = currentTime - tiempoInicioTrabajo;
    unsigned long tiempoRestante = TIEMPO_TRABAJO - tiempoTrabajando;
    Serial.println("⚙️  TRABAJANDO - Tiempo restante: " + String(tiempoRestante / 60000) + " min");
    
    if (tiempoTrabajando >= ADVERTENCIA_CALOR) {
      Serial.println("🔥 VERIFICAR TEMPERATURA DE COMPONENTES");
    }
  }
  
  Serial.print("Motor: " + String(motorRunning ? "ACTIVO" : "DETENIDO"));
  Serial.print(" | Dirección: " + String(direction > 0 ? "Horario" : "Antihorario"));
  Serial.println(" | Velocidad: " + String(currentSpeed) + " RPM");
  Serial.println("============================\n");
}

void processCommand(char cmd) {
  switch (cmd) {
    case 's':
    case 'S':
      toggleMotor();
      break;
      
    case 'd':
    case 'D':
      if (!estaEnfriando) {
        changeDirection();
      } else {
        Serial.println("❌ No se puede cambiar dirección durante enfriamiento");
      }
      break;
      
    case 'i':
    case 'I':
      mostrarEstadoSeguridad(millis());
      break;
      
    case 'f':
    case 'F':
      if (!estaEnfriando) {
        Serial.println("⚠️  Iniciando enfriamiento manual...");
        iniciarEnfriamiento(millis());
      }
      break;
      
    case 'h':
    case 'H':
      mostrarAyuda();
      break;
      
    default:
      Serial.println("Comando no reconocido. Envía 'h' para ayuda.");
      break;
  }
}

void toggleMotor() {
  if (!estaEnfriando) {
    motorRunning = !motorRunning;
    if (motorRunning) {
      Serial.println(">>> MOTOR INICIADO");
    } else {
      Serial.println(">>> MOTOR DETENIDO");
      myStepper.stopMotor();
    }
  } else {
    Serial.println("❌ Motor en pausa de enfriamiento - esperar " + 
                   String((TIEMPO_ENFRIAMIENTO - (millis() - tiempoInicioEnfriamiento)) / 60000) + " min");
  }
}

void changeDirection() {
  direction = -direction;
  Serial.println(">>> DIRECCIÓN: " + String(direction > 0 ? "Horario" : "Antihorario"));
}

void mostrarAyuda() {
  Serial.println("\n=== COMANDOS DISPONIBLES ===");
  Serial.println("s - Detener/Iniciar motor");
  Serial.println("d - Cambiar dirección");
  Serial.println("i - Mostrar información de seguridad");
  Serial.println("f - Forzar enfriamiento manual");
  Serial.println("h - Mostrar esta ayuda");
  Serial.println("============================\n");
}
