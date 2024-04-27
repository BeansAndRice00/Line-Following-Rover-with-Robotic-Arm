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
/*
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
*/
        m_r.speed(left_speed);
        m_l.speed(right_speed);

        //Thread::wait(1500);
    }
}

void bluetooth_thread()
{
    char bnum=0;
    bhit=0;
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
                                    if (currentState == ARM) {
                                        move_arm = 1;
                                        arm_commanded = ARM_MOTOR2_FORWARD;
                                    }
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                    if (currentState == ARM) 
                                    { 
                                        move_arm = 0;
                                        arm_commanded = ARM_MOTOR2_STOPPED;
                                    }
                                }
                                break;
                            case '6': //button 6 down arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_REVERSE << 8 | ROVER_MOTOR2_REVERSE;
                                    if (currentState == ARM) 
                                    { 
                                        move_arm = 1;
                                        arm_commanded = ARM_MOTOR2_REVERSE;
                                    }
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                    if (currentState == ARM) 
                                    { 
                                        move_arm = 0;
                                        arm_commanded = ARM_MOTOR2_STOPPED;
                                    }
                                }
                                break;
                            case '7': //button 7 left arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_REVERSE << 8 | ROVER_MOTOR2_FORWARD;
                                    if (currentState == ARM) {
                                        move_arm = 1;
                                        arm_commanded = ARM_MOTOR1_FORWARD;
                                    }
                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                    if (currentState == ARM) 
                                    { 
                                        move_arm = 0;
                                        arm_commanded = ARM_MOTOR1_STOPPED;
                                    }
                                }
                                break;
                            case '8': //button 8 right arrow
                                if (bhit=='1') {
                                    if (currentState == MANUAL) rover_commanded = ROVER_MOTOR1_FORWARD << 8 | ROVER_MOTOR2_REVERSE;
                                    if (currentState == ARM) {                                       
                                        move_arm = 1;
                                        arm_commanded = ARM_MOTOR1_REVERSE;                                       
                                    }

                                } else {
                                    if (currentState == MANUAL) rover_commanded = ROVER_STOPPED;
                                    if (currentState == ARM) 
                                    { 
                                        move_arm = 0;
                                        arm_commanded = ARM_MOTOR1_STOPPED;
                                    }
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
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
        /*
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
        */
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

void IR_thread() {
        //printf("%.2f\t", left.read());   // tab character
        //printf("%.2f\t", center.read()); // tab character
        //printf("%.2f\n", right.read());
		while(1) {
            motor_lock.lock();
			if (currentState == PATH_FIND) {
				// if on the line drive left and right at the same speed (left is CCW / right is CW)
				if (center.read() > LINETHRESHOLD)
				{   
					led1 = 1;
					if (currentState == PATH_FIND) rover_commanded = ROVER_MOTOR1_FORWARD << 8 | ROVER_MOTOR2_FORWARD;
				}

				// if the line is under the right sensor, adjust relative speeds to turn to the right
				else if (right.read() > LINETHRESHOLD)
				{
					led2 = 1;
					if (currentState == PATH_FIND) rover_commanded = ROVER_MOTOR1_FORWARD << 8 | ROVER_MOTOR2_REVERSE;
				}

				// if the line is under the left sensor, adjust relative speeds to turn to the left
				else if (left.read() > LINETHRESHOLD)
				{
					led4 = 1;
					if (currentState == PATH_FIND) rover_commanded = ROVER_MOTOR1_REVERSE << 8 | ROVER_MOTOR2_FORWARD;
				} else {
					if (currentState == PATH_FIND) rover_commanded = ROVER_STOPPED;
				}
			} 
            motor_lock.unlock();

            Thread::wait(75);
		}

}


void check_move_arm() {
    float change = 0.0f;
    while(1) {
        if (move_arm)
        {
            if (arm_commanded == ARM_MOTOR1_FORWARD || arm_commanded == ARM_MOTOR2_FORWARD || arm_commanded == ARM_MOTOR3_FORWARD) 
            {
            //printf("Condition 1\n");
                change = 0.03;
            }
            if (arm_commanded == ARM_MOTOR1_REVERSE || arm_commanded == ARM_MOTOR2_REVERSE || arm_commanded == ARM_MOTOR3_REVERSE) 
            {
            //printf("Condition 2\n");
                change = -0.03;
            }

            printf("Entered For loop to move base.\n");
            for (float i = base.read(); i >= 0.0f && i <= 1.0f; i += change) {
                if (arm_commanded == ARM_MOTOR1_FORWARD || arm_commanded == ARM_MOTOR1_REVERSE) {
                    base.write(i);
                    printf("Base moved.");
                }
                if (arm_commanded == ARM_MOTOR3_FORWARD || arm_commanded == ARM_MOTOR3_REVERSE) claw.write(i);
                if (!move_arm) {
                    break;
                }
            }
            printf("Entered For loop to move arm.\n");
            for (float i = arm_s2.read(); i >= 0.2f && i <= 0.8f; i += change) {
            if (arm_commanded == ARM_MOTOR2_FORWARD || arm_commanded == ARM_MOTOR2_REVERSE) {
                printf("Right arm servo moved.\n");
                arm_s1.write(i); 
                // Thread::wait(75*(10^(-3)));
                // 1.5f
                arm_s2.write(i);
                printf("Left arm servo moved.\n");
                }
            if (!move_arm) {
                break;
            }
        }
        }
        else
        {
            change = 0.0f;
            //Could also set arm_commanded to stopped...
        }
        
        //printf("%f\n", change);
        Thread::wait(100);
    }
}


int main() {
    blueTooth_init();
    serial_init();

    t1.start(serial_tx);
    t2.start(LeftRightMotor);
    t3.start(check_move_arm);
    t4.start(bluetooth_thread);
    t5.start(IR_thread);

    State prev_state = STANDBY;

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
                    /*
                    if (prev_state == PATH_FIND)
                    {
                        Thread::wait(125);
                        while (rover_commanded != ROVER_STOPPED) rover_commanded = ROVER_STOPPED;
                        prev_state = MANUAL;
                    }
                    */
                    if (autonomous_commanded == TRUE)   {currentState = PATH_FIND; autonomous_commanded = FALSE;}
                    else                                currentState = MANUAL;
                    break;
                case PATH_FIND:
                    //If user wants manual control, then manual. If an object is detected, then switch to Arm
                    if (manual_commanded == TRUE)       
                    {
                        currentState    = MANUAL;
                        prev_state      = PATH_FIND;
                    }
                    if (object_detected == TRUE)        currentState = ARM;
                    break;
                case ARM:
                    //If user wants manual control, then manual. If an object is detected, then switch to Arm
                    while (rover_commanded != ROVER_STOPPED) rover_commanded = ROVER_STOPPED;
                    if (manual_commanded == TRUE)       currentState = MANUAL;
                    if (autonomous_commanded == TRUE)   currentState = PATH_FIND;
                    if (object_detected == TRUE)        currentState = ARM;
                    break;
            }
            Thread::wait(50);
            if (currentState == STANDBY) { 
                led1 = 1;
                led2 = 0;
                led3 = 0;
                led4 = 0;
            }
            if (currentState == MANUAL) { 
                led1 = 0;
                led2 = 1;
                led3 = 0;
                led4 = 0;
            }
            if (currentState == PATH_FIND) { 
                led1 = 0;
                led2 = 0;
                led3 = 1;
                led4 = 0;
            }
            if (currentState == ARM) { 
                led1 = 0;
                led2 = 0;
                led3 = 0;
                led4 = 1;
            }
    }
}