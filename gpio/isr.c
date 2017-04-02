#include <stdio.h>
#include <pigpio.h>

#define INTERRUPT_PIN_L 4
#define INTERRUPT_PIN_R 5
unsigned long rcount = 0;
unsigned long lcount = 0;

void isr(int gpio, int level, uint32_t tick) 
{
    if(gpio == INTERRUPT_PIN_L) {
        lcount++;
    } else {
        rcount++;
    }
}

int main(int argc, char *argv[]) 
{
    if(gpioInitialise() < 0) {
        return -1;
    }

    if(gpioSetISRFunc(INTERRUPT_PIN_L, RISING_EDGE, 5, isr) != 0) {
        return -1;
    }
    if(gpioSetISRFunc(INTERRUPT_PIN_R, RISING_EDGE, 5, isr) != 0) {
        return -1;
    }

    while(1) {
        printf("lcount: %lu, rcount: %lu\n", lcount, rcount);
        sleep(1);
    }

    gpioTerminate();
    return 0;
}
