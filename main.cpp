#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library


DigitalOut led1(LED1);
DigitalOut led2(LED2);
Thread thread;
uLCD_4DGL uLCD(p13,p14,p15); //Tx, Rx, Rst

void led2_thread() {
    while (true) {
        led2 = !led2;
        Thread::wait(1000);
    }
}
 
int main() {
    thread.start(led2_thread);
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
    wait(0.5);
    wait(2);
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
