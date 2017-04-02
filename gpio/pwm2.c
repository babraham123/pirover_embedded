#include <wiringPi.h>
#include <stdio.h>

int main()
{
  if (wiringPiSetupGpio() == -1) {
    printf("cannot setup gpio.\n");
    return 1;
  }

  //pinMode(1, PWM_OUTPUT);
  pinMode(24, PWM_OUTPUT);
  //pwmSetMode(PWM_MODE_MS);
  //pwmSetClock(500);
  pwmSetRange(1024);
  int count = 0;

  while (1) {
    //pwmWrite(1, count);
    pwmWrite(24, count);
    count = (count + 1) % 1024;
    printf("count: %d\n", count);
    delay(10);
  }

  return 0;
}
