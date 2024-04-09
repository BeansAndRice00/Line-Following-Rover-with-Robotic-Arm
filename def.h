#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"     //Updated LCD Library

DigitalOut led1(LED1);
DigitalOut led2(LED2);

Thread thread;
Thread t1;
Thread t2;
Thread t3;
Thread t4;
Thread t5;

AnalogOut DACout(p18);

RawSerial pc(USBTX, USBRX);  // Initialize a serial port for USB communication
RawSerial bluemod(p28, p27);     // Initialize another serial port using pins p28 and p27

