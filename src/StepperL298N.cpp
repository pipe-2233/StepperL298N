#include "StepperL298N.h"

// Constructor
StepperL298N::StepperL298N(int steps, int p1, int p2, int p3, int p4) {
  stepsPerRevolution = steps;
  pin1 = p1;
  pin2 = p2;
  pin3 = p3;
  pin4 = p4;
  
  // Inicializar delay por defecto (lento)
  stepDelay = 50; // ms entre pasos
  
  // Definir secuencia de pasos
  stepSequence[0][0] = 1; stepSequence[0][1] = 0; stepSequence[0][2] = 1; stepSequence[0][3] = 0;
  stepSequence[1][0] = 0; stepSequence[1][1] = 1; stepSequence[1][2] = 1; stepSequence[1][3] = 0;
  stepSequence[2][0] = 0; stepSequence[2][1] = 1; stepSequence[2][2] = 0; stepSequence[2][3] = 1;
  stepSequence[3][0] = 1; stepSequence[3][1] = 0; stepSequence[3][2] = 0; stepSequence[3][3] = 1;
  
  // Configurar pines como salidas
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  
  // Inicializar todos los pines en LOW
  stopMotor();
}

// Establecer velocidad en RPM
void StepperL298N::setSpeed(int rpm) {
  if (rpm > 0) {
    stepDelay = 60000 / (stepsPerRevolution * rpm * 4);
    if (stepDelay < 1) stepDelay = 1;  // Mínimo 1ms
  }
}

// Mover un número específico de pasos (positivo = horario, negativo = antihorario)
void StepperL298N::step(int steps) {
  if (steps > 0) {
    stepClockwise(steps);
  } else if (steps < 0) {
    stepCounterClockwise(-steps);
  }
}

// Mover en sentido horario
void StepperL298N::stepClockwise(int steps) {
  for (int i = 0; i < steps; i++) {
    for (int j = 0; j < 4; j++) {
      executeStep(j);
      delay(stepDelay);
    }
  }
  stopMotor();
}

// Mover en sentido antihorario
void StepperL298N::stepCounterClockwise(int steps) {
  for (int i = 0; i < steps; i++) {
    for (int j = 3; j >= 0; j--) {
      executeStep(j);
      delay(stepDelay);
    }
  }
  stopMotor();
}

// Ejecutar un paso específico
void StepperL298N::executeStep(int stepIndex) {
  digitalWrite(pin1, stepSequence[stepIndex][0]);
  digitalWrite(pin2, stepSequence[stepIndex][1]);
  digitalWrite(pin3, stepSequence[stepIndex][2]);
  digitalWrite(pin4, stepSequence[stepIndex][3]);
}

// Detener motor (apagar todas las bobinas)
void StepperL298N::stopMotor() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}