# StepperL298N Library

Una librería simple y eficiente para controlar motores paso a paso usando el driver dual H-bridge L298N.

## ⚠️ ADVERTENCIAS IMPORTANTES DE HARDWARE

### 🔥 **RECALENTAMIENTO DEL MÓDULO L298N**
- El L298N puede alcanzar **temperaturas muy altas** durante el funcionamiento
- **SIEMPRE** usar un **disipador de calor** en el chip L298N
- Proporcionar **ventilación adecuada** al módulo
- **NO tocar** el chip L298N durante o después del funcionamiento
- Considerar usar un **ventilador** para aplicaciones de uso continuo

### 🔥 **RECALENTAMIENTO DEL MOTOR PASO A PASO**
- Los motores paso a paso se **calientan significativamente** durante el funcionamiento
- Esto es **NORMAL** pero requiere precauciones:
  - **NO** hacer funcionar continuamente por períodos prolongados sin pausa
  - Permitir **períodos de enfriamiento** entre sesiones de trabajo
  - **Monitorear** la temperatura del motor regularmente
  - Si el motor está **demasiado caliente para tocar**, detener inmediatamente

### 🔋 **ALIMENTACIÓN**
- Usar fuente de alimentación **adecuada** (generalmente 5V-12V para el motor)
- **NO** alimentar motores grandes directamente desde Arduino/ESP32
- Usar fuente de alimentación **externa** para el L298N

### ⚡ **PROTECCIÓN**
- Agregar **diodos de protección** si no están incluidos en el módulo
- Usar **capacitores de filtrado** en la alimentación
- **Nunca** desconectar el motor mientras está energizado

### 📋 **GUÍA COMPLETA DE SEGURIDAD**
**🔥 [LEE LA GUÍA COMPLETA DE SEGURIDAD](SAFETY.md) ANTES DEL USO 🔥**

Esta guía incluye procedimientos detallados, listas de verificación y medidas de emergencia.

## Características

- ✅ **API simple** y fácil de usar
- ✅ **Control de velocidad** configurable (RPM)
- ✅ **Movimiento bidireccional** (horario/antihorario)
- ✅ **Secuencia de pasos optimizada** para L298N
- ✅ **Ejemplos incluidos** (básico y avanzado)
- ✅ **Compatible** con ESP32, Arduino Uno, Nano, etc.
- ⚠️ **Advertencias de seguridad** incluidas

## Instalación

### Mediante Arduino Library Manager
1. Abrir Arduino IDE
2. Ir a **Sketch > Include Library > Manage Libraries**
3. Buscar "**StepperL298N**"
4. Hacer clic en **Install**

### Instalación manual
1. Descargar la librería
2. Extraer en la carpeta `libraries` de Arduino
3. Reiniciar Arduino IDE

## Conexiones

```
ESP32/Arduino    L298N
-----------------
Pin 32    ->     IN1
Pin 33    ->     IN2  
Pin 25    ->     IN3
Pin 26    ->     IN4
```

## Uso básico

```cpp
#include <StepperL298N.h>

// Crear instancia (200 pasos por revolución, pines de control)
StepperL298N myStepper(200, 32, 33, 25, 26);

void setup() {
  Serial.begin(115200);
  
  // Configurar velocidad en RPM
  myStepper.setSpeed(2.0);
}

void loop() {
  // Girar 100 pasos en sentido horario
  myStepper.step(100);
  delay(1000);
  
  // Girar 100 pasos en sentido antihorario  
  myStepper.step(-100);
  delay(1000);
}
```

## API Reference

### Constructor
```cpp
StepperL298N(int steps, int pin1, int pin2, int pin3, int pin4)
```
- `steps`: Pasos por revolución del motor (típicamente 200)
- `pin1-pin4`: Pines conectados a IN1, IN2, IN3, IN4 del L298N

### Métodos

#### `setSpeed(float rpm)`
Configura la velocidad del motor en revoluciones por minuto.
```cpp
myStepper.setSpeed(2.5);  // 2.5 RPM
```

#### `step(int steps)`
Mueve el motor un número específico de pasos.
- Valores positivos: sentido horario
- Valores negativos: sentido antihorario
```cpp
myStepper.step(50);   // 50 pasos horario
myStepper.step(-25);  // 25 pasos antihorario
```

#### `stepClockwise(int steps)`
Mueve el motor en sentido horario.
```cpp
myStepper.stepClockwise(100);
```

#### `stepCounterClockwise(int steps)`
Mueve el motor en sentido antihorario.
```cpp
myStepper.stepCounterClockwise(100);
```

#### `stopMotor()`
Detiene el motor y apaga todas las bobinas.
```cpp
myStepper.stopMotor();
```

## Ejemplos

La librería incluye tres ejemplos:

### BasicStepper
Ejemplo básico que muestra movimientos simples bidireccionales con advertencias de seguridad.

### AdvancedControl
Ejemplo avanzado con:
- Control por comandos serie
- Cambio de velocidad dinámico
- Movimiento continuo no bloqueante
- Advertencias de seguridad en tiempo real

### SafeOperation ⭐
Ejemplo con **monitoreo automático de seguridad**:
- Pausas automáticas cada 30 minutos
- Monitoreo de tiempo de funcionamiento
- Ciclos de enfriamiento obligatorios
- **Recomendado para uso industrial o continuo**

## Esquema de conexión

```
                    L298N Module
                   ┌─────────────┐
    Pin 32  ───────┤ IN1         │
    Pin 33  ───────┤ IN2         │
    Pin 25  ───────┤ IN3         │
    Pin 26  ───────┤ IN4         │
                   │             │
    +12V    ───────┤ +12V        │
    GND     ───────┤ GND         │
                   │             │
                   │ OUT1   OUT2 │───── Motor
                   │ OUT3   OUT4 │───── Paso a Paso
                   └─────────────┘
```

## Compatibilidad

- ✅ ESP32
- ✅ ESP8266  
- ✅ Arduino Uno/Nano/Mega
- ✅ STM32
- ✅ Cualquier microcontrolador compatible con Arduino

## Contribuir

¿Encontraste un bug o tienes una mejora? ¡Las contribuciones son bienvenidas!

1. Fork el repositorio
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## Autor

**Andre** - Tecnólogo en Electrónica Industrial  
Especializado en IoT, automatización industrial y sistemas embebidos.

## Agradecimientos

- Inspirado en la necesidad de una librería simple para L298N
- Basado en experiencia práctica en proyectos industriales
- Comunidad Arduino por el feedback y testing
