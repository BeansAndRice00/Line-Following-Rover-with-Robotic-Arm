#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library
#include "Motor.h"

#ifndef DEF_H
#define DEF_H

#define TRUE    1
#define FALSE   0

#define ARM_TX  p9
#define ARM_RX  p10

#define ROVER_TX  p13
#define ROVER_RX  p14

//Set of Commands to be used by Rover during Manual and Line-Following
    //Motor1 -> Left Motor
enum RoverCommand {
    ROVER_MOTOR1_FORWARD,     // 000
    ROVER_MOTOR1_REVERSE,     // 001
    ROVER_MOTOR2_FORWARD,     // 010
    ROVER_MOTOR2_REVERSE,     // 011
    ROVER_RESERVED,           // 100
};

enum ArmCommand {
    ARM_MOTOR1_FORWARD,     // 000
    ARM_MOTOR1_REVERSE,     // 001
    ARM_MOTOR2_FORWARD,     // 010
    ARM_MOTOR2_REVERSE,     // 011
    ARM_MOTOR3_FORWARD,     // 100
    ARM_MOTOR3_REVERSE,     // 101
    ARM_RESERVED,           // 110
};

// Enum to represent states
enum State {
    STANDBY,
    MANUAL,
    PATH_FIND,
    ARM 
};

enum BT_CONTROLLER {
    BUTTON1 = 1,
    BUTTON2,
    BUTTON3,
    BUTTON4,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT
};

volatile State currentState = PATH_FIND;

#define BLUE_TX p28
#define BLUE_RX p27

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

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
RawSerial blue(BLUE_TX, BLUE_RX);     // Initialize another serial port using pins p28 and p27

volatile char serialBuffer;
volatile char serialBuffer_old;


//Motor
Motor m(p23, p6, p5); // pwm, fwd, rev
//AnalogIn pot(p20);
volatile RoverCommand commanded    = ROVER_RESERVED;
volatile float position     = 0.5;
volatile float sample       = 0;

//State-Changing Variables
volatile bool bluetooth_connect     = FALSE;
volatile bool autonomous_commanded  = FALSE;

#endif 
