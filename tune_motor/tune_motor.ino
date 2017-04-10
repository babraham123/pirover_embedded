
#include <PID_v1.h>

// Tuning parameters
double mInput = 0.0, mOutput = 0.0, mSetpoint = 1.0;
PID mPID(&mInput, &mOutput, &mSetpoint, 80.0, 0.0, 0.75, DIRECT);
                                      // Kp, Ki, Kd

const unsigned long serialPing = 500; // ping interval in ms
unsigned long lastMessage = 0;

#define MOTOR 10 
#define ENCODER 2 
volatile long counter = 0;

void count() 
{
  counter++;
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    pinMode(MOTOR, OUTPUT);
    pinMode(ENCODER, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENCODER), count, RISING);

    unsigned long now = millis();
    lastMessage = now;

    double K = 1000.0 / (20 / (0.065 * 3.14159)); 
                  // (ms/s) / (ticksPerMeter)
                  // ticksPerMeter = ticksPerRev / MetersPerRev
    mPID.SetOutputLimits(0, 255);
    mPID.SetWheelParam(K);
    mPID.SetSampleTime(50);
    mPID.SetMode(AUTOMATIC);
    delay(10);
}

void loop()
{
    mPID.ComputeVelocity(counter);
    analogWrite(MOTOR, mOutput);

    unsigned long now = millis();
    if((now - lastMessage) > serialPing) {
        Serial.print(mInput);
        Serial.print(" ");
        Serial.println(mOutput);
        
        lastMessage = now;
    }

    if (Serial.available() > 0) { // check for new commands
        char inByte = char(Serial.read());
        double val = double(Serial.parseFloat());

        switch(inByte) {
        case 's' :
            mSetpoint = val;
            break;
        case 'p' :
            mPID.SetTunings(val, mPID.GetKi(), mPID.GetKd());
            break;
        case 'd' :
            mPID.SetTunings(mPID.GetKp(), mPID.GetKi(), val);
            break;
        case 'i' :
            mPID.SetTunings(mPID.GetKp(), val, mPID.GetKd());
            break;
        }
    }
}
