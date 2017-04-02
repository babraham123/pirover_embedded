#include <pigpio.h>
#include <stdio.h>

/*
gcc -Wall -pthread -o prog prog.c -lpigpio -lrt
sudo ./prog

12  PWM channel 0  All models but A and B
13  PWM channel 1  All models but A and B
18  PWM channel 0  All models
19  PWM channel 1  All models but A and B

40  PWM channel 0  Compute module only
41  PWM channel 1  Compute module only
45  PWM channel 1  Compute module only
52  PWM channel 0  Compute module only
53  PWM channel 1  Compute module only
*/

#define PWM_PIN_L 18

int main()
{
    if(gpioInitialise() < 0) { return 1; }

    // gpioSetMode(PWM_PIN_1, PI_OUTPUT);
    // gpioPWM(PWM_PIN_1, 0); 
    // gpioSetPWMfrequency(24, 1000); // Set GPIO24 to 1000Hz.

    int pwm = 0;
    while(1) {
        gpioHardwarePWM(PWM_PIN_L, 1000, pwm * 10000) // 1000Hz 75% dutycycle
        pwm = (pwm + 1) % 100;
        printf("pwm: %d\n", pwm);
        gpioDelay(10000); // .01s
    }

    gpioTerminate();
    return 0;
}

