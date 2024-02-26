#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library
#include "wave_player.h"
#include "SDFileSystem.h"


DigitalOut led1(LED1);
DigitalOut led2(LED2);
Thread thread;
Thread t1;
Thread t2;
Thread t3;
Thread t4;
Thread t5;

uLCD_4DGL uLCD(p13,p14,p15); //Tx, Rx, Rst

SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card

// mutex to make the lcd lib thread safe
Mutex lcd_mutex;

PwmOut RGBLED_r(p23);
PwmOut RGBLED_g(p24);
PwmOut RGBLED_b(p25);

AnalogOut DACout(p18);
wave_player waver(&DACout);

void led2_thread() {
    while (true) {
        led2 = !led2;
        Thread::wait(1000);
    }
}

void thread1() // lcd1
{
    while(true) {       // thread loop
        for (int i = 0; i < 15; i++)
        {
            lcd_mutex.lock();
            //uLCD.printf("Thread1 count: %d", 10000);
            uLCD.filled_circle(32, 32, i, BLUE);
            lcd_mutex.unlock();
            Thread::wait(100);
        }
        lcd_mutex.lock();
        uLCD.filled_circle(32, 32, 15, BLACK);
        lcd_mutex.unlock();

        Thread::wait(1000);
    }
}

// Thread 2
// print counter into third line and wait for 0,5s
void thread2() // lcd2
{
    while(true) {       // thread loop
        for (int i = 0; i < 15; i++)
        {
            lcd_mutex.lock();
            //uLCD.printf("Thread1 count: %d", 10000);
            uLCD.filled_circle(96, 96, i, RED);
            lcd_mutex.unlock();
            Thread::wait(100);
        }
        lcd_mutex.lock();
        uLCD.filled_circle(96, 96, 15, BLACK);
        lcd_mutex.unlock();

        Thread::wait(1000);
    }
}

// Thread 5
// RGB LED
void thread3()
{
    while(true) {         // thread loop
        RGBLED_r = 2 *(0.5 + (rand() % 11)/20.0);
        RGBLED_g = 0.5 + (rand() % 11)/20.0;
        RGBLED_b = 0.5 + (rand() % 11)/20.0;
        Thread::wait(500);    // wait 1.5s
    }
}

void SD_audio_thread() {
    while(true) {
        FILE *wave_file;
        // printf("\r\n\nHello, wave world!\n\r");
        Thread::wait(1000);
        wave_file=fopen("/sd/burp_x_8k.wav","r");
        if(wave_file==NULL) printf("file open error!\n\n\r");
        else printf("file open!\n\n\r");

        printf("file about to play.\n\n\r");
        waver.play(wave_file);
        printf("file has been played.\n\n\r");

        fclose(wave_file);

        Thread::wait(1000);
    }
}
 
RawSerial pc(USBTX, USBRX);  // Initialize a serial port for USB communication
RawSerial bluemod(p28, p27);     // Initialize another serial port using pins p28 and p27

// Function to handle receiving data from the device serial port
void bluetooth_thread()
{
    char bred=0;
    char bgreen=0;
    char bblue=0;
    RGBLED_r = RGBLED_g = RGBLED_b = 0;
    printf("bluetooth starting\n\n\r");
    while(1) {
            printf("After while, before lock\n\n\r");
        //lcd_mutex.lock();
                    printf("After lock\n\n\r");

        if (bluemod.getc()=='!') {
                                printf("First getc\n\n\r");

            if (bluemod.getc()=='C') { //color data packet
                                            printf("Second getc\n\n\r");

                bred = bluemod.getc(); // RGB color values
                                printf("Third getc\n\n\r");

                bgreen = bluemod.getc();
                                printf("Fourth getc\n\n\r");

                bblue = bluemod.getc();
                                printf("Fifth getc\n\n\r");

                if (bluemod.getc()==char(~('!' + 'C' + bred + bgreen + bblue))) { //checksum OK?
                    RGBLED_r = bred/255.0; //send new color to RGB LED PWM outputs
                    RGBLED_g = bgreen/255.0;
                    RGBLED_b = bblue/255.0;
                }
            }
        }
        printf("bluetooth ending\n\n\r");
        //lcd_mutex.unlock();
    }
}

void blueTooth_init()
{
    pc.baud(9600);  // Set the baud rate for the PC serial port
    bluemod.baud(9600); // Set the baud rate for the device serial port

    // Attach the pc_recv function to handle interrupts on the PC serial port
    //pc.attach(&pc_recv, Serial::RxIrq);
    // Attach the dev_recv function to handle interrupts on the device serial port
    //bluemod.attach(&dev_recv, Serial::RxIrq);
}

int main() {

    blueTooth_init();

    // thread.start(led2_thread);
    t1.start(thread1); // lcd1
    t2.start(thread2); // lcd2
    // t3.start(thread3);
    t4.start(SD_audio_thread);
    t5.start(bluetooth_thread);

    wait(0.5);
    wait(2);
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
