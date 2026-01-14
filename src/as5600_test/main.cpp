#include <Arduino.h>
#include <SimpleFOC.h>

// as5600 sensor quick config
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

void setup() {
  // monitoring port
  Serial.begin(115200);

  // init magnetic sensor hardware
  sensor.init();
    Serial.println("AS5600 ready");
  _delay(1000);
}
void loop(){
  // update the sensor values 
  sensor.update();

  // display the angle and velocity
  Serial.print("Position: ");
  Serial.println(sensor.getAngle());
  Serial.print("Velocity: ");
  Serial.println(sensor.getVelocity());
}
