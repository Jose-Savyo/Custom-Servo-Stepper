# Custom-Servo-Stepper: Closed-Loop FOC Control

This project documents the development of a high-performance servo motor system by converting a standard NEMA 17 stepper motor into a closed-loop brushless-style servo. This is achieved using the **SimpleFOC** library, an **AS5600** magnetic encoder for feedback, and an **Arduino Mega** for control logic.

## 🛠 Hardware Stack
* **Motor:** NEMA 17 Stepper (1.8°/step, 50 pole pairs).
* **Driver:** L298N Dual H-Bridge (Configured for 4PWM).
* **Sensor:** AS5600 Magnetic Encoder (I2C interface).
* **Controller:** Arduino Mega 2560.
* **Power Supply:** 12V DC.

---

## 🚀 Development Roadmap

The project is structured into three progressive stages to ensure system stability and calibration accuracy.

### 1. Open-Loop Control (`/firmware/open-loop`)
Initial stage to validate the power electronics and motor phase wiring.
* **Goal:** Verify sinusoidal waveform generation and driver thermal limits.
* **Mode:** `MotionControlType::angle_openloop`.
* **Behavior:** Motor moves to commanded angles without position correction.

### 2. Encoder Validation (`/firmware/encoder-test`)
Standalone diagnostic for the AS5600 magnetic sensor.
* **Goal:** Ensure reliable I2C communication and magnet alignment.
* **Key Feature:** Implementation of `MagneticSensorI2C` to monitor cumulative angle (multi-turn tracking).
* **Connection:** Uses Pins 20 (SDA) and 21 (SCL) on the Arduino Mega.

### 3. Closed-Loop FOC Servo (`/firmware/closed-loop`)
Full implementation of **Field Oriented Control (FOC)**.
* **Goal:** Active position holding and high-precision motion.
* **Status:** Achieved `MOT: PP check: OK!`, confirming perfect synchronization between the 50 pole pairs and the encoder.
* **Control Architecture:** Cascaded PID loops (Position -> Velocity -> Torque/Voltage).



---

## 📉 Control Parameters & Tuning
The system is currently tuned for stability and rigidity using the following parameters:

| Parameter | Value | Description |
| :--- | :--- | :--- |
| `PID_velocity.P` | 0.5 | Proportional gain for velocity response |
| `PID_velocity.I` | 60.0 | Integral gain to eliminate steady-state error |
| `P_angle.P` | 20.0 | Proportional gain for position rigidity |
| `voltage_limit` | 12.0V | Maximum voltage allowed to the L298N |
| `velocity_limit` | 50.0 | Maximum angular velocity |

---

## 💻 Operation via Commander
The firmware includes a Serial Interface (115200 baud) for real-time interaction:

* **Move to Position:** Send `T` + angle in radians (e.g., `T3.14` for 180°).
* **Tune Gains:** Use `M` commands to adjust PID values on the fly (e.g., `MP30` to increase position stiffness).
* **Status:** Send `M` alone to receive a full diagnostic report of the motor state.

---
