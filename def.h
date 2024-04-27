#include "Mutex.h"
#include "mbed.h"
#include "rtos.h"
//#include "uLCD_4DGL.h"     //Updated LCD Library
#include "Motor.h"
#include "Servo.h"
#include "ultrasonic.h"

#ifndef DEF_H
#define DEF_H

#define TRUE    1
#define FALSE   0

#define ARM_TX  p9
#define ARM_RX  p10

#define ROVER_TX  p13
#define ROVER_RX  p14

void RightMotor(); 
void LeftMotor();


//DEGUB
#define STATE_MACHINE
#define ROVER_THREADS
#define ARM_THREADS

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
Mutex motor_lock; 


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
    ARM_MOTOR1_STOPPED,    
    ARM_MOTOR2_STOPPED,    
    ARM_MOTOR3_STOPPED,
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

volatile State currentState = STANDBY;

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
Thread t6;

AnalogOut DACout(p18);

RawSerial pc(USBTX, USBRX);             // Initialize a serial port for USB communication
RawSerial arm(ARM_TX, ARM_RX);          // Initialize a serial port for ARM communication
RawSerial rover(ROVER_TX, ROVER_RX);    // Initialize a serial port for ROVER communication
RawSerial blue(BLUE_TX, BLUE_RX);       // Initialize a serial port for Bluetooth communication

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

#define COLOR_THRESHOLD 1000.0
#define BASE_COLOR      100.0
#define IR_SCALE        1000.0
//volatile AnalogIn IR_in;


//ARM
Servo base(p21);    //Base Servo
Servo arm_s1(p25);  //Arm Servo
Servo arm_s2(p26);  
Servo claw(p22);    //Claw Servo

volatile bool move_arm;
volatile char bhit;


//Linesensor
AnalogIn left(p15);   // initialize a left sensor object on p15
AnalogIn center(p16); // initialize a center sensor object on p16
AnalogIn right(p17);  // initialize a right sensor object on p17
#define LINETHRESHOLD 0.8

//Ultrasonic Sensor
void alert(int distance);
ultrasonic mu_left(p6, p7, .1, 1, &alert);
ultrasonic mu_right(p12, p13, .1, 1, &alert);
//ultrasonic mu_center(p6, p7, .1, 1, &alert);

#endif 
