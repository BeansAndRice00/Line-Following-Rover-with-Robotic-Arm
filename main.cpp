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
    //led1 = !led1;
    while(1)
    {
        while(arm.readable()) {
            pc.printf("Rx-ing\n");
            temp = arm.getc();
            if (temp=='1') led2 = 1;
            if (temp=='0') led2 = 0;
            else 
            {
                pc.printf("%c\n", temp);
            }
            
        }
        Thread::wait(50);
    }
}

//Adapted from Lab 4
void serial_tx()
{
    char serialBuffer = 0;
    //led1 = !led1;
    while(1)
    {
        while(rover.writeable() && serialBuffer != serialBuffer_old) {
            pc.printf("Tx-ing\n");
            rover.putc(serialBuffer);
            serialBuffer_old = serialBuffer;
        }
        Thread::wait(50);
    }
}

void serial_init()
{
    arm.baud(9600);
    arm.attach(&serial_rx, Serial::RxIrq);

    rover.baud(9600);

    serialBuffer        = RESERVED;
    serialBuffer_old    = RESERVED;

    pc.baud(9600);
    //pc.attach(&serial_rx, Serial::RxIrq);
}


int main() {
    pc.printf("Start\n");
    blueTooth_init();
    serial_init();

    t1.start(serial_tx);
    t2.start(serial_rx);

    Thread::wait(1000);
    serialBuffer = char(MOTOR1_REVERSE);
    Thread::wait(1000);
    serialBuffer = char(MOTOR3_REVERSE);
    Thread::wait(2000);
    while (true) {
        //led1 = !led1;
        Thread::wait(500);
    }
}
