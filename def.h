#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library

#ifndef DEF_H
#define DEF_H


#define ARM_TX  p9
#define ARM_RX  p10

#define ROVER_TX  p13
#define ROVER_RX  p14

enum ArmCommand {
    MOTOR1_FORWARD,     // 000
    MOTOR1_REVERSE,     // 001
    MOTOR2_FORWARD,     // 010
    MOTOR2_REVERSE,     // 011
    MOTOR3_FORWARD,     // 100
    MOTOR3_REVERSE,     // 101
    RESERVED,           // 110
};

#define BLUE_TX p28
#define BLUE_RX p27

DigitalOut led1(LED1);
DigitalOut led2(LED2);

Thread thread;
Thread t1;
Thread t2;
Thread t3;
Thread t4;
Thread t5;

AnalogOut DACout(p18);

RawSerial pc(USBTX, USBRX);  // Initialize a serial port for USB communication
RawSerial arm(ARM_TX, ARM_RX);  // Initialize a serial port for USB communication
RawSerial rover(ROVER_TX, ROVER_RX);  // Initialize a serial port for USB communication
RawSerial bluemod(BLUE_TX, BLUE_RX);     // Initialize another serial port using pins p28 and p27

volatile char serialBuffer;
volatile char serialBuffer_old;

#endif 
