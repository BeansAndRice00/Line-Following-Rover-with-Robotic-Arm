
#include "cmsis_os.h"
#include "mbed.h"
#include "rtos.h"
#include "SDFileSystem.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"     //Updated LCD Library
// #include "mbed_stats.h" // Include the statistics header


SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card

AnalogOut DACout(p18);

wave_player waver(&DACout);

PwmOut RGBLED_r(p25);
PwmOut RGBLED_g(p24);
PwmOut RGBLED_b(p23);

uLCD_4DGL uLCD(p13,p14,p15); //Tx, Rx, Rst

// mutex to make the lcd lib thread safe
Mutex lcd_mutex;

// int threadCount = 0;

// RGB LED thread
void RGB_thread(void const *args){
    while(true) {         // thread loop
        // 0.5 + (rand() % 11)/20.0; -> varies RGB values from 0.5 to 1.05
        // x % 11 gives if x is divisible by 11 (0 if divisible, gives remainder if not divisible)
        // Thus x % 11 gives a range of values form 0 (for x = 0,11) to 10 (for x = 10)
        // 0.5 + (0-10)/20.0 -> 0.5 (for x % 11 = 0) to 1.00 (for x % 11 = 10)
        // take away 0.5 + for wider range? Worked. LED was previously only lighting up to be white.
        // after taking awway 0.5, was able to see visible color changes in RGB.
        // printf("RGB LED start.\n\n\r");
        RGBLED_r = (rand() % 11)/20.0;
        RGBLED_g = (rand() % 11)/20.0;
        RGBLED_b = (rand() % 11)/20.0;
        // printf("RGB LED end.\n\n\r");
        Thread::wait(1667);    // wait 1.5s
        // printf("RGB LED end of delay 1.\n\n\r");
        // wait_us(1500000);    // wait 1.5s
        
        /*lcd_mutex.lock();
        printf("lcd MUTEX LOCKED.\n\n\r");
        threadCount++;
        lcd_mutex.unlock();
        printf("lcd MUTEX UNLOCKED.\n\n\r");
        // Thread::wait(1667);    // wait 1.5s
        printf("RGB LED end of delay 2.\n\n\r");
        Thread::yield();*/
    }
}

void SD_audio_thread(void const *args) {
    while(true) {
        FILE *wave_file;
        // printf("\r\n\nHello, wave world!\n\r");
        Thread::wait(1000);
        wave_file=fopen("/sd/why-are.wav","r");
        if(wave_file==NULL) printf("file open error!\n\n\r");

        // printf("file about to play.\n\n\r");
        waver.play(wave_file);
        // printf("file has been played.\n\n\r");

        fclose(wave_file);

        /*lcd_mutex.lock();
        printf("lcd MUTEX LOCKED.\n\n\r");
        threadCount++;
        lcd_mutex.unlock();
        printf("lcd MUTEX UNLOCKED.\n\n\r");*/
        Thread::wait(1000);
        Thread::yield();
    }
}

void lcd_1(void const *args)
{
    // while(true) {       // thread loop
    // printf("lcd_1 started.\n\n\r");
    /*for (int i = 0; i < 15; i++)
    {
        lcd_mutex.lock();
        printf("lcd_1 MUTEX LOCKED.\n\n\r");
        //uLCD.printf("Thread1 count: %d", 10000);
        uLCD.filled_circle(32, 32, i, BLUE);
        lcd_mutex.unlock();
        printf("lcd_1 MUTEX UNLOCKED.\n\n\r");
        Thread::wait(100);
    }*/
    lcd_mutex.lock();
    // printf("lcd_1 MUTEX LOCKED.\n\n\r");
    // uLCD.filled_circle(32, 32, 15, BLUE);
    uLCD.filled_circle(96, 96, 15, RED);
    // threadCount++;
    lcd_mutex.unlock();
    // printf("lcd_1 MUTEX UNLOCKED.\n\n\r");
    // Thread::wait(1000);
    // printf("lcd_1 ended.\n\n\r");
    // Thread::yield();
    return;
    // }
}

// Thread 2
// print counter into third line and wait for 0,5s
void lcd_2(void const *args)
{
    // while(true) {       // thread loop
        /*for (int i = 0; i < 15; i++)
        {
            lcd_mutex.lock();
            //uLCD.printf("Thread1 count: %d", 10000);
            uLCD.filled_circle(96, 96, i, RED);
            lcd_mutex.unlock();
            Thread::wait(100);
        }
        lcd_mutex.lock();
        // uLCD.printf("Count: %d\n", threadCount); // Correctly format the string
        // uLCD.printf("Hello World\n"); // Correctly format the string
        uLCD.filled_circle(36, 36, 30, BLUE);
        // threadCount++;
        lcd_mutex.unlock();
        Thread::wait(1000);*/

        lcd_mutex.lock();
        
        // Clear a larger area to ensure the whole number is covered.
        // This example assumes a small area. Adjust the size as needed.
        // Let's say each character is roughly 8 pixels wide and 8 pixels tall,
        // and we want to clear an area wide enough for about 10 characters.
        /*uLCD.filled_rectangle(0, 0, 80, 16, 0x0000); // Adjust dimensions as needed

        // Now, relocate the cursor to the beginning of the cleared area and print the new count.
        // Ensure to locate the cursor in terms of character positions, not pixels.
        // For example, if you clear a rectangle starting at pixel (0,0), 
        // setting the cursor to (0,0) would also make sense for text.
        uLCD.locate(0, 0);
        uLCD.color(BLUE); // Set text color if needed
        uLCD.textbackground_color(BLACK); // Ensure the background color of text is set to black
        // uLCD.printf("Count: %d\n", threadCount); // Print count, extra spaces help clear previous characters
        // threadCount++;
        */
        uLCD.filled_circle(36, 36, 30, BLUE);
        lcd_mutex.unlock();
        Thread::wait(1000); // Update the display every second
        Thread::yield();
        // Thread::set_priority(priority);
        return;
    // }
}

int main()
{
    Thread t1(RGB_thread); //start thread1
    t1.set_priority(osPriorityNormal);
    Thread t2(SD_audio_thread); //start thread2
    t2.set_priority(osPriorityNormal);
    Thread t3(lcd_1); //start thread3
    t3.set_priority(osPriorityNormal);
    Thread t4(lcd_2); //start thread4
    t4.set_priority(osPriorityNormal);
    while(true) {
        /*FILE *wave_file;
        printf("\r\n\nHello, wave world!\n\r");
        Thread::wait(1000);
        wave_file=fopen("/sd/alarm_beep.wav","r");
        if(wave_file==NULL) printf("file open error!\n\n\r");

        printf("file about to play.\n\n\r");
        waver.play(wave_file);
        printf("file has been played.\n\n\r");

        fclose(wave_file);*/
        // uLCD.filled_circle(32, 32, 20, BLUE);
        // uLCD.printf("uLCD circle printed.\n"); // Test text
    }
}