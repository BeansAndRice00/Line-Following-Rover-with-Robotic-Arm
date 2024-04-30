# Line Following Rover with Robotic Arm
-ECE 4180 Spring 2024 Final Project, Georgia Institute of Technology
-Project Members: Devansh Singh, Nicholas Graham, Tejasvi V Yadavalli
-This project implements a versatile rover equipped with a robotic arm, capable of line following. It utilizes the ARM mbed platform for development.


![Rover with Robotic Arm](rover_with_arm.jpg)

# Project Idea:
A line-following rover with a servo-controlled robotic arm. The rover can move forwards, backwards, left, and right in the manual state. The rover also has an autonomous line following state where it follows a dark line with IR sensors. The servo-controlled arm can move up, down, left, and right in the arm state. The purpose of the arm is to move objects that are in the way of the rover. The purpose of the project was to create a smaller scale version of an excavator with an autonomous line following function to simulate a method of moving a large excavator without human control. This line following function could be beneficial in storing large numbers of excavators at a time in warehouses.


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
