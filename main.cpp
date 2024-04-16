#include "Thread.h"
#include "def.h"

float util_speedConversion(enum RoverCommand commanded)
{
    switch (commanded) {
        case ROVER_MOTOR1_FORWARD: //number button 1
            return 0.5;
            break;
        case ROVER_MOTOR1_REVERSE: //number button 1
            return -0.5;
            break;
        case ROVER_MOTOR2_FORWARD: //number button 1
            return 0.5;
            break;
        case ROVER_MOTOR2_REVERSE: //number button 1
            return -0.5;
            break;
        default:
            return 0.0;
            break;
    }

}

void LeftMotor() 
{
    while(true) {       

        //pc.printf("Left Motor Running\n");
        led1 = !led1;
        sample      = util_speedConversion(commanded); 
        //sample      = -1.0; 
        position    = sample;
        m_r.speed(position);
        //wait(0.02);
        Thread::wait(1000);
    }
}


void RightMotor() 
{
    while(true) {       

        //pc.printf("Right Motor Running\n");
        led2 = !led2;
        sample      = util_speedConversion(commanded); 
        //sample      = -1.0; 
        position    = sample;
        m_r.speed(position);
        //wait(0.02);
        Thread::wait(1000);
    }
}

// From: https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/
//See Control Pad GUI
//Button 1 -> Manual
//Button 2 -> Path_Find
//Button 3 -> Reserved, Standby --> Useless, don't use
    // Up (5), Down (6), Left (7), Right (8)
    // On hit, will move motor. On release, will stop motor
    // Up is pos, down negative. 
    // Left moves left motor reverse, right forward  

void bluetooth_thread()
{
    char bnum=0;
    char bhit=0;
    while(1) {
        //("In Blue\n\n\r");
        //Thread::wait(100);
        //bnum = blue.getc();
        //if (blue.readable())
            //led4 = 1;

        //Thread::wait(50);
            
        if (blue.readable()) {
            led4 = !led4;
            if (bluetooth_connect == FALSE) bluetooth_connect = TRUE;
            if (blue.getc()=='!') {
                if (blue.getc()=='B') { //button data packet
                    bnum = blue.getc(); //button number
                    bhit = blue.getc(); //1=hit, 0=release
                    if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                        switch (bnum) {
                            case '1': //number button 1
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '2': //number button 2
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '3': //number button 3
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '4': //number button 4
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '5': //button 5 up arrow
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '6': //button 6 down arrow
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '7': //button 7 left arrow
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            case '8': //button 8 right arrow
                                if (bhit=='1') {
                                    //add hit code here
                                } else {
                                    //add release code here
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

        Thread::wait(50);
       
    }
}

void blueTooth_init()
{
    blue.baud(9600); // Set the baud rate for the device serial port
}

//From Lab 4
void serial_rx()
{
    char temp = 0;
    if(rover.readable()) {
        //pc.printf("Rx-ing\n");
        //led3 = ~led3;
        wait(0.050);
        temp = rover.getc();
        if (temp==ARM_MOTOR3_REVERSE) 
        {
            //led2 = 1;
            led3 = 0;
        }
        if (temp==ARM_MOTOR3_FORWARD) 
        {
            led3 = 1;
            //led2 = 0;
        }
        //pc.printf("%d\n", temp);

        //wait(0.050);
    }
    //Thread::wait(50);
}

//Adapted from Lab 4
//char serialBuffer = 0;
void serial_tx()
{
    serialBuffer = ARM_MOTOR3_REVERSE;
    //led1 = !led1;
    while(1)
    {
        char temp = 0;
        //while(rover.writeable() && serialBuffer != serialBuffer_old) {
        if (rover.writeable()){ 
            //pc.printf("Tx-ing\n");
            if (serialBuffer == ARM_MOTOR3_REVERSE) serialBuffer = ARM_MOTOR3_FORWARD;
            else serialBuffer = ARM_MOTOR3_REVERSE;

            rover.putc(serialBuffer);
            serialBuffer_old = serialBuffer;
        } else {
            //pc.printf("Not Writeable\n");
        }

        Thread::wait(1000);
    }
}

void serial_init()
{
    arm.baud(9600);
    arm.attach(&serial_rx, Serial::RxIrq);

    rover.baud(9600);
    rover.attach(&serial_rx, Serial::RxIrq);

    serialBuffer        = ARM_RESERVED;
    serialBuffer_old    = ARM_RESERVED;

    pc.baud(9600);
    //pc.attach(&serial_rx, Serial::RxIrq);
}


int main() {
    //pc.printf("Start\n");
    blueTooth_init();
    serial_init();

    t1.start(serial_tx);

    t2.start(LeftMotor);
    t3.start(RightMotor);
    t4.start(bluetooth_thread);

    //Test Inputs to Serial, Motor
    Thread::wait(1000);
    serialBuffer = char(ARM_MOTOR1_REVERSE);
    Thread::wait(1000);
    serialBuffer = char(ARM_MOTOR3_REVERSE);
    Thread::wait(2000);

    commanded = ROVER_MOTOR1_FORWARD;
    Thread::wait(5000);
    commanded = ROVER_MOTOR1_REVERSE;

    State currentState = PATH_FIND;

    while (true) {
        while (true) {
            // State machine logic
            switch (currentState) {
                case STANDBY:
                    //If Bluetooth Connects during init, then start with Manual Control
                    if (bluetooth_connect == TRUE) currentState = MANUAL;
                    else currentState = STANDBY;
                    break;
                case MANUAL:
                    //If autonomous line-following commanded, switch to Pathfinding
                    if (autonomous_commanded == TRUE) currentState = PATH_FIND;
                    else currentState = MANUAL;
                    break;
                case PATH_FIND:
                    //currentState = ;
                    break;
                case ARM:
                    //currentState = ;
                    break;
            }
    }


        Thread::wait(500);
    }
}
