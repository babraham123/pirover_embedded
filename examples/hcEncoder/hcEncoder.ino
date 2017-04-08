
double Input;
double K = 1000.0 / (20.0 / (0.065 * 3.14159)); // (ms/s) * ticksPerMeter

unsigned long lastCompute = 0;
int Encoder = 2; 
int Motor = 10;
volatile unsigned long counter = 0;

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

    pinMode(Encoder, INPUT);
    attachInterrupt(digitalPinToInterrupt(Encoder), count, RISING);
    lastCompute = millis();

    pinMode(Motor, OUTPUT);
    analogWrite(Motor, 0); // [0, 255]
}

void loop()
{
    // velocity calculation
    int temp;
    unsigned long now = millis();
    double dt = double(now - lastCompute);
    lastCompute = now;
    temp = counter;
    counter = 0;
    Input = K * double(temp) / dt; // m/s

    Serial.print(Input);

    if (Serial.available() > 0) { // check for new commands
        char inByte = (char)Serial.read();
        int val = Serial.parseInt();

        if(inByte == 's') {
            analogWrite(Motor, val);
        }
    }
    delay(200);
}
