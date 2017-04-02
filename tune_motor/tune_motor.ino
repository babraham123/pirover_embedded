
#include <PID_v1.h>

// Tuning parameters
float Kp = 0;
float Ki = 10;
float Kd = 0;
double Input, Output;
double Setpoint = 0.1;
PID mPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

double K = 1000.0 / 99.4; // (ms/s) * ticksPerMeter

const unsigned long serialPing = 500; // ping interval in ms
unsigned long now = 0;
unsigned long lastMessage = 0;
unsigned long lastCompute = 0;
#define BUFFER_SIZE 8 // total message size = BUFFER_SIZE + 2

int Motor = 10; 
int Encoder = 2; 
volatile unsigned long Counter = 0;

void count() 
{
  Counter++;
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println('A');
    delay(300);
  }
  int a = Serial.read();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    mPID.SetMode(AUTOMATIC);
    pinMode(Motor, OUTPUT);
    pinMode(Encoder, INPUT);
    attachInterrupt(digitalPinToInterrupt(Encoder), count, RISING);

    lastCompute = millis();
    lastMessage = lastCompute;

    // establishContact();
}

void loop()
{
    // velocity calculation
    int temp;
    now = millis();
    double dt = (double)(now - lastCompute);
    lastCompute = now;
    temp = Counter;
    Counter = 0;
    
    Input = K * ((double)temp) / dt; // m/s

    mPID.Compute();
    
    // Output = map(Output, 0, 5, 0, 255); // map to right scale
    analogWrite(Motor, Output);

    if((now - lastMessage) > serialPing) {
        // char buffer[BUFFER_SIZE];
        // snprintf(buffer, BUFFER_SIZE, "%f", Input);
        // Serial.print(buffer);
        // Serial.print(' ');
        // snprintf(buffer, BUFFER_SIZE, "%f", Output);
        // Serial.println(buffer);

        // Serial.write('s');
        // Serial.write(Input);

        Serial.print(Input);
        Serial.print(" ");
        Serial.println(Output);
        
        lastMessage = now;
    }

    if (Serial.available() > 0) { // check for new commands
        char inByte = (char)Serial.read();
        float val = Serial.parseFloat();

        switch(inByte) {
        case 's' :
            Setpoint = (double)val;
            break;
        case 'p' :
            Kp = val;
            mPID.SetTunings(Kp, Ki, Kd);
            break;
        case 'd' :
            Kd = val;
            mPID.SetTunings(Kp, Ki, Kd);
            break;
        case 'i' :
            Ki = val;
            mPID.SetTunings(Kp, Ki, Kd);
            break;
        }
        // thirdSensor = map(digitalRead(2), 0, 1, 0, 255);
    }
}
