#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library


DigitalOut led1(LED1);
DigitalOut led2(LED2);
Thread thread;
Thread t1;
Thread t2;
Thread t3;
//Thread t4;
uLCD_4DGL uLCD(p13,p14,p15); //Tx, Rx, Rst

// mutex to make the lcd lib thread safe
Mutex lcd_mutex;

PwmOut RGBLED_r(p23);
PwmOut RGBLED_g(p24);
PwmOut RGBLED_b(p25);


void led2_thread() {
    while (true) {
        led2 = !led2;
        Thread::wait(1000);
    }
}

void thread1()
{
    while(true) {       // thread loop
        lcd_mutex.lock();
        uLCD.printf("Thread1 count: %d", 10000);
        lcd_mutex.unlock();
        Thread::wait(1000);
    }
}

// Thread 2
// print counter into third line and wait for 0,5s
void thread2()
{
    while(true) {       // thread loop
        lcd_mutex.lock();
        //LCD.set_font((unsigned char*) Arial_9);
        uLCD.printf("Thread 2 count");
        lcd_mutex.unlock();
        Thread::wait(500); // wait 0.5s
    }
}

// Thread 5
// RGB LED
void thread3()
{
    while(true) {         // thread loop
        RGBLED_r = 0.5 + (rand() % 11)/20.0;
        RGBLED_g = 0.5 + (rand() % 11)/20.0;
        RGBLED_b = 0.5 + (rand() % 11)/20.0;
        Thread::wait(1667);    // wait 1.5s
    }
}
/*
// Thread 6
// Speaker
void thread4()
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
*/

 
int main() {
    thread.start(led2_thread);
/*
    // basic printf demo = 16 by 18 characters on screen
    uLCD.printf("\nHello uLCD World\n"); //Default Green on black text
    uLCD.printf("\n  Starting Demo...");
    uLCD.text_width(4); //4X size text
    uLCD.text_height(4);
    uLCD.color(RED);
    for (int i=10; i>=0; --i) {
        uLCD.locate(1,2);
        uLCD.printf("%2D",i);
        wait(.5);
    }
    uLCD.cls();
    uLCD.printf("Change baudrate......");
    uLCD.baudrate(3000000); //jack up baud rate to max for fast display
    //if demo hangs here - try lower baud rates
    //
    // printf text only full screen mode demo
    uLCD.background_color(BLUE);
    uLCD.cls();
    uLCD.locate(0,0);
    uLCD.color(WHITE);
    uLCD.textbackground_color(BLUE);
    uLCD.set_font(FONT_7X8);
    uLCD.text_mode(OPAQUE);
    int i=0;
    while(i<64) {
        if(i%16==0) uLCD.cls();
        uLCD.printf("TxtLine %2D Page %D\n",i%16,i/16 );
        i++; //16 lines with 18 charaters per line
    }
*/
    t1.start(thread1);
    t2.start(thread2);
    t3.start(thread3);
    //t4.start(thread4);
    wait(0.5);
    wait(2);
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
