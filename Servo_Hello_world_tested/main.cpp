#include "mbed.h"
#include "Servo.h"

// Initialize serial communication with the host computer
Serial pc(USBTX, USBRX); // USBTX and USBRX are pin names for the UART transmit and receive pins

Servo base(p21);
Servo claw(p22);

int main() {
    pc.baud(9600);  // Set baud rate to 9600, ensure it matches your terminal's settings
    // // base.period_ms(20);  // Set the PWM period to 20 ms
    // claw.period_ms(20);
    // wait(2);

    base.write(0.5);
    claw.write(0.5);

    // // Start at the middle position (1.5 ms pulse width)
    // base.pulsewidth_us(1500);  
    // claw.pulsewidth_us(1250);  
    // wait(1);

    while(1) {
        // Decreasing the pulse width from 2000 microseconds back to 1000 microseconds
        //for(int pw = 2000; pw >= 1000; pw -= 50) {
        for(float i = 1.0; i >= 0.0; i -= 0.03) {
            if (i < 0.8 && i > 0.0) {
                
            }
            claw.write(i);
            base.write(i);
            // base.pulsewidth_us(pw);
            // claw.pulsewidth_us(pw);
            // pc.printf("Decreasing to %d us\n", i);  // Print to serial port
            wait(0.075);
        }
        // Increasing the pulse width from 1500 microseconds to 2000 microseconds
        // for(int pw = 1000; pw <= 2000; pw += 50) {
        for(float i = 0.0; i <= 1.0; i += 0.03) {
            // base.pulsewidth_us(pw);
            // claw.pulsewidth_us(pw);
            if (i < 0.8 && i > 0.0) {
                 
            }
            claw.write(i);
            base.write(i);
            //claw.write(i);
            // pc.printf("Increasing to %d us\n", i);  // Print to serial port
            wait(0.075);
        }
    }
}
