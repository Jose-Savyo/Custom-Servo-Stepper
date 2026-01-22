#include <Arduino.h>
#include <SimpleFOC.h>

#define   _MON_TARGET 0b1000000  // monitor target value

// Stepper Motor instance (Nema 17 50 pair of poles)
StepperMotor motor = StepperMotor(50);

// Driver L298N: IN1, IN2, IN3, IN4, EN1, EN2
// Use pins PWM: 2, 3, 5, 6 for phases
StepperDriver4PWM driver = StepperDriver4PWM(2, 3, 5, 6, 7, 8);

// As5600 sensor quick config
MagneticSensorI2C encoder = MagneticSensorI2C(AS5600_I2C);

// Comander Interface
Commander command = Commander(Serial);
void onMotor(char* cmd){
  command.motor(&motor, cmd);
}
void doTarget(char* cmd) { 
  command.scalar(&motor.target, cmd);
}

void setup(){
  // Init magnetic encoder
  encoder.init();

  // Link the motor to the sensor
  motor.linkSensor(&encoder);

  // Chose FOC modulation
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // Power supply electrical voltage [V]
  driver.voltage_power_supply = 12;
  driver.init();
  motor.linkDriver(&driver); // Link the motor to the driver

  // Set control loop type to be used
  //motor.controller = MotionControlType::torque;
  motor.controller = MotionControlType::angle;
  // Controller configuration based on the control type
  motor.PID_velocity.P = 0.01;
  motor.PID_velocity.I = 60;
  motor.PID_velocity.D = 0.;
  // Default elevtrical voltage_power_supply
  motor.voltage_limit = 12;

  // Velocity low pass filtering time constant
  motor.LPF_velocity.Tf = 0.01;

  // Angle loop controller
  motor.P_angle.P = 20;
  // Angle loop velocity limit
  motor.velocity_limit = 50;

  // Use monitoring with Serial for motor init
  Serial.begin(115200);
  motor.useMonitoring(Serial);

  motor.voltage_sensor_align = 5;

  // Initialise motor
  motor.init();

  // Align encoder and start FOC
  motor.initFOC();

  // Set the initial target value
  motor.target = 0;

  // Define the motor id
  command.add('M', onMotor, "motor");
  command.add('T', doTarget, "target angle");

  // Runb user commands to configure motor
  Serial.println(F("Motor commands sketch | Initial motion control > torque/voltage : target 2V."));

  _delay(1000);
}

void loop(){

  // Interative setting FOC phase voltage
  motor.loopFOC();

  // Interative function setting the outter loop target
  // Velocity, position or voltage
  // If target not set in parameter uses motor.target variable
  motor.move();

  // User command line
  command.run();
}