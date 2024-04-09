#include "def.h"


void thread1() // lcd1
{
    while(true) {       // thread loop

        Thread::wait(1000);
    }
}

// Thread 2
// print counter into third line and wait for 0,5s
void thread2() // lcd2
{
    while(true) {       // thread loop

        Thread::wait(1000);
    }
}

// Function to handle receiving data from the device serial port
void bluetooth_thread()
{
    char bred=0;
    char bgreen=0;
    char bblue=0;
    //RGBLED_r = RGBLED_g = RGBLED_b = 0;
    printf("bluetooth starting\n\n\r");
    while(1) {
        if (bluemod.getc()=='!') {
            if (bluemod.getc()=='C') { //color data packet
                if (bluemod.getc()==char(~('!' + 'C' + bred + bgreen + bblue))) { //checksum OK?
                }
            }
        }
        printf("bluetooth ending\n\n\r");
    }
}

void blueTooth_init()
{
    bluemod.baud(9600); // Set the baud rate for the device serial port
}

//From Lab 4
void serial_rx()
{
    char temp = 0;
    led1 = !led1;
    while(arm.readable()) {
        temp = arm.getc();
        if (temp=='1') led2 = MOTOR1_FORWARD;
        if (temp=='0') led2 = 0;
    }
}

//Adapted from Lab 4
void serial_tx()
{
    char serialBuffer = 0;
    led1 = !led1;
    while(arm.writeable() && serialBuffer != serialBuffer_old) {
        arm.putc(serialBuffer);
    }
}

void serial_init()
{
    arm.baud(9600);
    arm.attach(&serial_rx, Serial::RxIrq);
    serialBuffer        = RESERVED;
    serialBuffer_old    = RESERVED;
}


int main() {

    blueTooth_init();
    serial_init();

    t1.start(serial_tx);
    
    Thread::wait(1000);
    serialBuffer = MOTOR1_FORWARD;

    Thread::wait(2000);
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
