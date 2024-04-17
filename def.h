#include "Mutex.h"
#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library
#include "Motor.h"
#include "Servo.h"

#ifndef DEF_H
#define DEF_H

#define TRUE    1
#define FALSE   0

#define ARM_TX  p9
#define ARM_RX  p10

#define ROVER_TX  p13
#define ROVER_RX  p14

void RightMotor(); 
void RightMotor();

// From: https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/
//See Control Pad GUI
//Button 1 -> Manual
//Button 2 -> Path_Find
//Button 3 -> Reserved, Standby --> Useless, don't use
    // Up (5), Down (6), Left (7), Right (8)
    // On hit, will move motor. On release, will stop motor
    // Up is pos, down negative. 
    // Left moves left motor reverse, right forward  
void bluetooth_thread();
void blueTooth_init();

//From Lab 4
void serial_rx();

//Adapted from Lab 4
//char serialBuffer = 0;
void serial_tx();

void serial_init();

Mutex serial_lock; 



//Set of Commands to be used by Rover during Manual and Line-Following
    //Motor1 -> Left Motor
enum RoverCommand {
    ROVER_STOPPED,            // 000
    ROVER_MOTOR1_FORWARD,     // 001
    ROVER_MOTOR1_REVERSE,     // 010
    ROVER_MOTOR2_FORWARD,     // 011
    ROVER_MOTOR2_REVERSE,     // 100
    ROVER_RESERVED,           // 101
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

volatile State currentState = MANUAL;

#define BLUE_TX p28
#define BLUE_RX p27

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

Thread thread;
Thread t1;
Thread t2;
Thread t3;
Thread t4;
Thread t5;

AnalogOut DACout(p18);

RawSerial pc(USBTX, USBRX);             // Initialize a serial port for USB communication
RawSerial arm(ARM_TX, ARM_RX);          // Initialize a serial port for USB communication
RawSerial rover(ROVER_TX, ROVER_RX);    // Initialize a serial port for USB communication
RawSerial blue(BLUE_TX, BLUE_RX);       // Initialize another serial port using pins p28 and p27

volatile char serialBuffer;
volatile char serialBuffer_old;


//Motor
Motor m_l(p23, p6, p5);     // pwm, fwd, rev
Motor m_r(p24, p12, p11);   // pwm, fwd, rev
//AnalogIn pot(p20);
volatile int            rover_commanded    = ROVER_RESERVED;
volatile ArmCommand     arm_commanded      = ARM_RESERVED;
volatile float left_speed       = 0.0;
volatile float right_speed      = 0.0;

//State-Changing Variables
volatile bool bluetooth_connect     = FALSE;
volatile bool autonomous_commanded  = FALSE;
volatile bool manual_commanded      = FALSE;
volatile bool object_detected       = FALSE;


#endif 
