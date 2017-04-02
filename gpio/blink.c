#include <wiringPi.h>

int main(void)
{
  wiringPiSetup();
  int pin = 24;
  pinMode(pin, OUTPUT);
  for(;;) {
    digitalWrite(pin, HIGH); 
    delay(500);
    digitalWrite(pin, LOW);
    delay(500);
  }
  return 0;
}

/*
git clone git://git.drogon.net/wiringPi
cd ~/wiringPi
./build
gpio -v
gpio readall

gcc -Wall -o blink blink.c -lwiringPi
sudo ./blink
*/