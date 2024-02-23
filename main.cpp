#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
//#include "wave_player.h"
#include "uLCD_4DGL.h"     //Updated LCD Library
//#include "cylonbyc.h"
//#include "Speaker.h"
//#include "SDFileSystem.h"

uLCD_4DGL LCD(p13,p14,p15); //Tx, Rx, Rst;
PwmOut Speaker(p26);
PwmOut RGBLED_r(p23);
PwmOut RGBLED_g(p24);
PwmOut RGBLED_b(p25);
BusOut leds(LED1,LED2,LED3,LED4);

// mutex to make the lcd lib thread safe
Mutex lcd_mutex;

Thread t1;

// Thread 1
// print counter into first line and wait for 1 s
void thread1(void const *args)
{
    while(true) {       // thread loop
        lcd_mutex.lock();
        LCD.printf("Thread1 count: %d");
        lcd_mutex.unlock();
        Thread::wait(1000);
    }
}

// Thread 2
// print counter into third line and wait for 0,5s
void thread2(void const *args)
{
    while(true) {       // thread loop
        lcd_mutex.lock();
        //LCD.set_font((unsigned char*) Arial_9);
        LCD.printf("Thread 2 count");
        lcd_mutex.unlock();
        Thread::wait(500); // wait 0.5s
    }
}

// Thread 5
// RGB LED
void thread5(void const *args)
{
    while(true) {         // thread loop
        RGBLED_r = 0.5 + (rand() % 11)/20.0;
        RGBLED_g = 0.5 + (rand() % 11)/20.0;
        RGBLED_b = 0.5 + (rand() % 11)/20.0;
        Thread::wait(1667);    // wait 1.5s
    }
}
// Thread 6
// Speaker
void thread6(void const *args)
{
    while(true) {         // thread loop
        Speaker.period(1.0/800.0);
        Speaker = 0.01;
        Thread::wait(1000);    // wait 1.0s
        Speaker.period(1.0/969.0);
        Speaker = 0.01;
        Thread::wait(1000);    // wait 1.0s
    }
}

int main()
{
    LCD.cls();
    LCD.background_color(RED);
    while(1){;}

    t1.start(thread1);

    /*
    Thread t1(thread1); //start thread1
    Thread t2(thread2); //start thread2
    Thread t5(thread5); //start thread5
    */
    //Thread t6(thread6); //start thread6

    while(true) {       // main is the next thread
        lcd_mutex.lock();
        LCD.printf("contrast");
        lcd_mutex.unlock();
        Thread::wait(500);   // wait 0.5s
    }
}