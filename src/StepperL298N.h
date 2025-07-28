#ifndef StepperL298N_h
#define StepperL298N_h

#include "Arduino.h"

class StepperL298N {
  private:
    int pin1, pin2, pin3, pin4;  // Pines de conexión
    int stepDelay;               // Delay entre pasos
    int stepsPerRevolution;      // Pasos por revolución
    int stepSequence[4][4];      // Secuencia de pasos
    
  public:
    // Constructor
    StepperL298N(int steps, int pin1, int pin2, int pin3, int pin4);
    
    // Métodos públicos
    void setSpeed(int rpm);           // Establecer velocidad en RPM
    void step(int steps);             // Mover un número específico de pasos
    void stepClockwise(int steps);    // Mover en sentido horario
    void stepCounterClockwise(int steps); // Mover en sentido antihorario
    void stopMotor();                 // Detener motor (apagar bobinas)
    
  private:
    // Métodos privados
    void executeStep(int stepIndex);  // Ejecutar un paso específico
};

#endif