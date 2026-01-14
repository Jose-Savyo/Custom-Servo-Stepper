#include <Arduino.h>
#include <SimpleFOC.h>

// Stepper motor instance (50 pares de polos para NEMA 17)
StepperMotor motor = StepperMotor(50);

// Driver L298N: IN1, IN2, IN3, IN4, EN1, EN2
// Use pinos PWM do Mega: 2, 3, 5, 6 para as fases
StepperDriver4PWM driver = StepperDriver4PWM(2, 3, 5, 6, 7, 8);

Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&motor.target, cmd); }

void setup() {
  // Configuração do Driver
  driver.voltage_power_supply = 12;
  driver.init();
  motor.linkDriver(&driver);

  // Limites para malha aberta
  motor.voltage_limit = 5;   // Aumente para 5V se o motor não tiver força
  motor.velocity_limit = 10; // Velocidade máxima em rad/s
  
  // Controle de ângulo em malha aberta
  motor.controller = MotionControlType::angle_openloop;

  // Inicializa apenas o hardware do motor
  motor.init();

  command.add('T', doTarget, "target angle");

  Serial.begin(115200);
  Serial.println("Motor em Malha Aberta Pronto!");
  Serial.println("Digite T + valor (ex: T6.28 para uma volta)");
  _delay(1000);
}

void loop() {
  // Em malha aberta, loopFOC não faz nada, mas mantemos para consistência
  motor.loopFOC(); 
  motor.move();
  command.run();
}
