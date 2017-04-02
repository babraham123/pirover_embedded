// https://www.youtube.com/watch?v=9848QETGd_k

// Using HC-020K Photoelectric encoder on the Dual Axis Reducer Motor 
// with 100nF capacitor on the encoder
int encoder_pin = 2; 
volatile unsigned int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(encoder_pin, INPUT);
  attachInterrupt(0, do_count, RISING);
}

void loop() {
  // delay
  Serial.print("Motor Speed: ");
  int rotation = (counter/20) * 60;
  Serial.print(rotation, DEC);
  Serial.println(" RPM");
  counter = 0;
}

void do_count() {
  counter++;
}

