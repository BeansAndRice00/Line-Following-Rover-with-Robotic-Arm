# mbed Rover with Robotic Arm and Line Following

This project implements a versatile rover equipped with a robotic arm, capable of line following. It utilizes the ARM mbed platform for development.

![Rover with Robotic Arm](rover_with_arm.jpg)

## Features

- **Rover Movement**: The rover can move forward, backward, turn left, and turn right.
- **Robotic Arm Control**: The robotic arm can be controlled to move up, down, left, and right
- **Line Following**: Incorporates IR sensors for line following, allowing the rover to autonomously follow lines.
- **Modular Design**: Easily expandable and customizable for additional functionalities.

## Hardware Components

- **mbed LPC1768**: The heart of the system, controls all functions.
- **DC Motors and Motor Driver**: For rover movement.
- **Servo Motors**: For the robotic arm.
- **IR Sensors**: for line following.
- **Power Supply**: Batteries or external power source.
- **Chassis and Wheels**: Physical structure for the rover.
- **Batteries**: to power the motor driver and the mbed
- **Bluetooth LE UART Module**: for remote control

## Software Components

- **mbed RTOS**: Provides the operating system and hardware abstraction layer for threading and interrupts.
- **mbed APIs**: Libraries for motor control, sensor interfacing, and communication.
- **Control Algorithms**: Implementations for rover movement, arm control, and line following.
- **Line Following Algorithm**: Determines rover behavior based on sensor input.


## License

This project is licensed under the MIT License.
