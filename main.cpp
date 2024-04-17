#include "Thread.h"
#include "def.h"

float util_speedConversion(int commanded)
{
    switch ((enum RoverCommand) commanded) {
        case ROVER_MOTOR1_FORWARD:
            return 0.5;
            break;
        case ROVER_MOTOR1_REVERSE:
            return -0.5;
            break;
        case ROVER_MOTOR2_FORWARD:
            return 0.5;
            break;
        case ROVER_MOTOR2_REVERSE:
            return -0.5;
            break;
        case ROVER_STOPPED:
            return 0.0;
            break;
        default:
            return 0.0;
            break;
    }

}

void LeftRightMotor() 
{
    while(true) {       

        //led1 = !led1;
        left_speed      = util_speedConversion(rover_commanded >> 8); 
        right_speed     = util_speedConversion(0x00FF & rover_commanded); 

/*
        serial_lock.lock();
        pc.printf("left speed: %f\n", left_speed);
        pc.printf("right speed: %f\n", right_speed);
        serial_lock.unlock();
*/
//Testing Code
        if (left_speed == 0.5)
        {
            led1 = 1;
            led2 = 0;
            led3 = 0;
            led4 = 0;

        }
        if (left_speed == -0.5)
        {
            led1 = 0;
            led2 = 1;
            led3 = 0;
            led4 = 0;
        }
        if (right_speed == 0.5)
        {
            led1 = 0;
            led2 = 0;
            led3 = 1;
            led4 = 0;
        }
        if (right_speed == -0.5)
        {
            led1 = 0;
            led2 = 0;
            led3 = 0;
            led4 = 1;
        }

        m_r.speed(left_speed);
        m_l.speed(right_speed);

        //Thread::wait(1500);
    }
}

void bluetooth_thread()
{
    char bnum=0;
    char bhit=0;
    while(1) {
        if (blue.readable()) {
            if (bluetooth_connect == FALSE) bluetooth_connect = TRUE; //We connected for the first time.
            //led4 = !led4;
            if (blue.getc()=='!') {
                if (blue.getc()=='B') { //button data packet
                    bnum = blue.getc(); //button number
                    bhit = blue.getc(); //1=hit, 0=release
                    if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                        switch (bnum) {
                            case '1': //number button 1
                                if (bhit=='1') {
                                    currentState = MANUAL;
                                } else {
                                    //add release code here
                                }
                                break;
                            case '2': //number button 2
                                if (bhit=='1') {
                                    currentState = PATH_FIND;
                                } else {
                                    //add release code here
                                }
                                break;
                            case '3': //number button 3
                                if (bhit=='1') {
                                    currentState = ARM;
                                } else {
                                    //add release code here
                                }
                                break;
                            case '4': //number button 4
                                if (bhit=='1') {
                                    //Nothing
                                } else {
                                    //add release code here
                                }
                                break;
                            case '5': //button 5 up arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_FORWARD << 8 | ROVER_MOTOR2_FORWARD;
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                }
                                break;
                            case '6': //button 6 down arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_REVERSE << 8 | ROVER_MOTOR2_REVERSE;
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                }
                                break;
                            case '7': //button 7 left arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_REVERSE << 8 | ROVER_MOTOR2_FORWARD;
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                }
                                break;
                            case '8': //button 8 right arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_FORWARD << 8 | ROVER_MOTOR2_REVERSE;
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
        /*
        serial_lock.lock();
        pc.printf("Rover Commanded: %x\n", rover_commanded);
        pc.printf("State: %x\n", currentState);
        serial_lock.unlock();
        */
        //Thread::wait(1500);
    }
}

void blueTooth_init()
{
    blue.baud(9600); // Set the baud rate for the device serial port
}

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
            //led3 = 0;
        }
        if (temp==ARM_MOTOR3_FORWARD) 
        {
            //led3 = 1;
            //led2 = 0;
        }
        //pc.printf("%d\n", temp);

        //wait(0.050);
    }
    //Thread::wait(50);
}

void serial_tx()
{
    serialBuffer = ARM_MOTOR3_REVERSE;
    while(1)
    {
        char temp = 0;
        if (rover.writeable()){ 
            if (serialBuffer == ARM_MOTOR3_REVERSE) serialBuffer = ARM_MOTOR3_FORWARD;
            else serialBuffer = ARM_MOTOR3_REVERSE;

            rover.putc(serialBuffer);
            serialBuffer_old = serialBuffer;
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
    blueTooth_init();
    serial_init();

    t1.start(serial_tx);
    t2.start(LeftRightMotor);
    t4.start(bluetooth_thread);

    State currentState = MANUAL;

    while (true) {
            // State machine logic
            switch (currentState) {
                case STANDBY:
                    //If Bluetooth Connects during init, then start with Manual Control
                    if (bluetooth_connect == TRUE)      currentState = MANUAL; 
                    else                                currentState = STANDBY;
                    break;
                case MANUAL:
                    //If autonomous line-following commanded, switch to Pathfinding
                    if (autonomous_commanded == TRUE)   {currentState = PATH_FIND; autonomous_commanded = FALSE;}
                    else                                currentState = MANUAL;
                    break;
                case PATH_FIND:
                    //If user wants manual control, then manual. If an object is detected, then switch to Arm
                    if (manual_commanded == TRUE)       currentState = MANUAL;
                    if (object_detected == TRUE)        currentState = ARM;
                    break;
                case ARM:
                    //If user wants manual control, then manual. If an object is detected, then switch to Arm
                    if (manual_commanded == TRUE)       currentState = MANUAL;
                    if (autonomous_commanded == TRUE)   currentState = PATH_FIND;
                    if (object_detected == TRUE)        currentState = ARM;
                    break;
            }
            Thread::wait(500);
    }
}
