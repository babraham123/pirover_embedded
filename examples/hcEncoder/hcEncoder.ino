
double Input;
double K = 1000.0 / 99.4; // (ms/s) * ticksPerMeter

const unsigned long serialPing = 500; // ping interval in ms
unsigned long lastCompute = 0;
int Encoder = 2; 
volatile unsigned long Counter = 0;

void count() 
{
  Counter++;
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
}

void loop()
{
    // velocity calculation
    int temp;
    unsigned long now = millis();
    double dt = double(now - lastCompute);
    lastCompute = now;
    temp = Counter;
    Counter = 0;
    Input = K * double(temp) / dt; // m/s

    Serial.print(Input);
    delay(200);
}
