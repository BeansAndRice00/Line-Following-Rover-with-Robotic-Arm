#include "mbed.h"
#include "Servo.h"
#include "rtos.h"

RawSerial  pc(USBTX, USBRX);
RawSerial  dev(p28,p27);
DigitalOut led1(LED1);
DigitalOut led4(LED4);
Servo base(p21);
Servo s2(p25);
Servo s3(p24);
Servo s4(p23);

void servo_reset() {
    base.write(0.5);
    s2.write(0.5);
    s3.write(0.5);
    s4.write(0.5);
}

void dev_recv()
{
    led1 = !led1;
    while(dev.readable()) {
        pc.putc(dev.getc());
    }
}

void pc_recv()
{
    led4 = !led4;
    while(pc.readable()) {
        dev.putc(pc.getc());
    }
}

void arm_blutooth(void const *args){
    printf("In thread.");
    char bnum;
    char bhit;
    while(true) {         // thread loop
        while(dev.readable()) {
            printf("Button Clicked!");
            if (dev.getc() == '!') {
                if (dev.getc() == 'B') {
                    bnum = dev.getc(); // button number obtained
                    bhit = dev.getc(); // 1 = hit, 0 = release
                    printf("%c", bnum);
                    printf("%c", bhit);
                        switch(bnum) {
                            case '7': // left arrow button hit 
                                if (bhit == '1') {
                                    printf("Base should be moving to the left.");
                                    //for (float i = 1.0; i >= 0.0; i -= 0.03) {
                                    base.write(base.read()-0.03);
                                    Thread::wait(750);
                                    //}
                                }
                                break;
                            case '8': // roght arrow button hit
                                if (bhit == '1') {
                                    printf("down button hit");
                                    // for(float i = 0.0; i <= 1.0; i += 0.03) {
                                    base.write(base.read()+0.03);
                                    Thread::wait(750);
                                    // }
                                }
                                break;
                            default:
                                printf("Neither up or down button hit");
                                break;
                        }
                }
            }
        }
        //SERVO.lock();
        //LCD.lock();
        //printf("Mutex lock for servos has happened.");
        //printf("Potential uLCD code");
        /*uLCD.locate(1,6); // example LCD line
        SERVO.unlock();
        LCD.unlock();*/
        //printf("Thread ended.");
    }
}
int main()
{
    Thread t1(arm_blutooth);
    pc.baud(9600);
    dev.baud(9600);

    //pc.attach(&pc_recv, Serial::RxIrq);
    //dev.attach(&dev_recv, Serial::RxIrq);

    while(1) {
        Thread::yield();
        //servo_reset();
    }
}
