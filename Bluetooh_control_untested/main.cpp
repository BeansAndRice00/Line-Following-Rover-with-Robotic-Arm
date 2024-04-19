#include "cmsis_os.h"
#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library
// #include "mbed_stats.h" // Include the statistics header
// #include "Servo.h"

// bluetooth pins
Serial blue(p9,p10);

// uLCD pins
uLCD_4DGL uLCD(p13,p14,p15);

// servo pins
// used pwm outs since Servo.h write function only sets servo position
// thus cannot turn off servo using Servo.h
// can turn off servos by changing pulse width using pwm out
PwmOut s1(p26);
PwmOut s2(p25);
PwmOut s3(p24);
PwmOut s4(p23);

// mutex to lock servo pwm
// Not needed since this demo code is not using multiple threads for servos. 
// Put here just in case for later use.
Mutex SERVO;

// lock for uLCD resources.
// Not needed since this demo not using multiple threads to display arm info on LCD. 
// Put here just in case for later use.
Mutex LCD;

// turn off servos
void servo_off(PwmOut s) {
    // set output duty-cycle of pwm output to 0%
    s.write(0.0f);
}

// bluetooth thread
void arm_blutooth(void const *args){
    char bnum;
    char bhit;
    while(true) {         // thread loop
        while(!blue.readable()) {
            printf("Bluetooth not readable.");
            // turn off servos when bluetooth disconnected to save power
            servo_off(s1);
            servo_off(s2);
            servo_off(s3);
            servo_off(s4);
            Thread::yield();
        }
        SERVO.lock();
        LCD.lock();
        printf("Mutex lock for servos has happened.");
        if (blue.getc() == '!') {
            if (blue.getc() == 'B') {
                bnum = blue.getc(); // button number obtained
                bhit = blue.getc(); // 1 = hit, 0 = release
                    switch(bnum) {
                        case '5': // up arrow button 
                            if (bhit == '1') {
                                printf("up button hit");
                                // depending on hardware, will increase pwm output pulse width
                                // pwmout_pulsewidth(&_pwm, seconds);
                                // pwmout_pulsewidth_ms(&_pwm, ms);
                                // pwmout_pulsewidth_us(&_pwm, us); -> most probably will use this one
                                // which servo(s) on/off? hardware dependent -> servo_off
                                // Increase by how much? need to test
                            }
                            break;
                        case '6': // down arrow button
                            if (bhit == '1') {
                                printf("down button hit");
                                // depending on hardware, will decrease pwm output pulse width
                                // pwmout_pulsewidth(&_pwm, seconds);
                                // pwmout_pulsewidth_ms(&_pwm, ms);
                                // pwmout_pulsewidth_us(&_pwm, us); -> most probably will use this one
                                // which servo(s) on/off? hardware dependent -> servo_off
                                // Decrease by how much? need to test
                            }
                            break;
                        default:
                            printf("Neither up or down button hit");
                            break;
                    }
            }
        }
        printf("Potential uLCD code");
        uLCD.locate(1,6); // example LCD line
        SERVO.unlock();
        LCD.unlock();
        printf("Thread ended.");
    }
}

int main()
{
    Thread t1(arm_blutooth); //start thread1
    t1.set_priority(osPriorityNormal);
    while(true) {
        // can change some vars here if we want to keep track of info on arm on
        // EX: how many objects we removed
        // can display on LCD (in this case, don't need mutex lock because only this loop would use uLCD)
    }
}