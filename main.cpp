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

void serial_init()
{
    pc.baud(9600);  // Set the baud rate for the PC serial port
}

int main() {

    blueTooth_init();


    //t1.start(thread1); // lcd1

    wait(2);
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
