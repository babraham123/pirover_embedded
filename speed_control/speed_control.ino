
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <PID_v1.h>

// Tuning parameters
float rKp = 0;
float rKi = 10;
float rKd = 0;
float lKp = 0;
float lKi = 10;
float lKd = 0;
double rInput, rOutput, lInput, lOutput;
double rSetpoint = 0;
double lSetpoint = 0;
PID rPID(&rInput, &rOutput, &rSetpoint, rKp, rKi, rKd, DIRECT);
PID lPID(&lInput, &lOutput, &lSetpoint, lKp, lKi, lKd, DIRECT);

const double K = 1000.0 / 99.4; // (ms/s) / ticksPerMeter
            // ticksPerMeter = ticksPerRev / MetersPerRev

// const int sampleRate = 1;
const unsigned long serialPing = 500; // ping interval in ms
unsigned long lastMessage = 0;
unsigned long lastCompute = 0;

int rMotor = 10; 
int lMotor = 11; 

int rEncoder = 2; 
int lEncoder = 3; 
volatile int rCounter = 0;
volatile int lCounter = 0;
int rCounterTotal = 0;
int lCounterTotal = 0;

void cmd_vel_r_cb(const std_msgs::Float32& cmd_msg)
{
    rSetpoint = (double)cmd_msg.data;
}

void cmd_vel_l_cb(const std_msgs::Float32& cmd_msg)
{
    lSetpoint = (double)cmd_msg.data;
}

ros::NodeHandle nh;

std_msgs::Int16 encoder_msg;
ros::Publisher pub_r("rwheel", &encoder_msg);
ros::Publisher pub_l("lwheel", &encoder_msg);

ros::Subscriber<std_msgs::UInt16> sub_r("rwheel_vtarget", &cmd_vel_r_cb);
ros::Subscriber<std_msgs::UInt16> sub_l("lwheel_vtarget", &cmd_vel_l_cb);

void count_r() 
{
  rCounter++;
}

void count_l() 
{
  lCounter++;
}

// TODO: switch to Int16 / int

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    rPID.SetMode(AUTOMATIC);
    // rPID.SetSampleTime(sampleRate);
    lPID.SetMode(AUTOMATIC);
    // lPID.SetSampleTime(sampleRate);

    pinMode(rEncoder, INPUT);
    attachInterrupt(digitalPinToInterrupt(rEncoder), count_r, RISING);
    pinMode(lEncoder, INPUT);
    attachInterrupt(digitalPinToInterrupt(lEncoder), count_l, RISING);

    lastCompute = millis();
    lastMessage = lastCompute;

    pinMode(rMotor, OUTPUT);
    pinMode(lMotor, OUTPUT);

    nh.initNode();
    nh.advertise(pub_r);
    nh.advertise(pub_l);
    nh.subscribe(sub_r);
    nh.subscribe(sub_l);
}

void loop()
{
    // velocity calculation
    int rtemp, ltemp;
    unsigned long now = millis();
    double dt = (double)(now - lastCompute);
    lastCompute = now;
    rtemp = rCounter;
    rCounter = 0;
    ltemp = lCounter;
    lCounter = 0;

    rInput = K * ((double)rtemp) / dt; // m/s
    lInput = K * ((double)ltemp) / dt;
    rCounterTotal += rtemp;
    lCounterTotal += ltemp;
    
    rPID.Compute();
    lPID.Compute();
    
    // rOutput = map(rOutput, 0, 5, 0, 255); // map to right scale
    analogWrite(rMotor, rOutput);
    analogWrite(lMotor, lOutput);

    if((now - lastMessage) > serialPing) { // send odometry
        encoder_msg.data = rCounterTotal;
        pub_r.publish(&encoder_msg);
        encoder_msg.data = lCounterTotal;
        pub_l.publish(&encoder_msg);
        
        lastMessage = now;
    }

    nh.spinOnce();
}

    // roscore
    // rostopic pub rwheel_vtarget std_msgs/Float32  <speed>

    // myPID.SetOutputLimits(0, 255);

    // adaptive tuning
    // myPID.SetTunings(aggKp, aggKi, aggKd);

    // noInterrupts();
    // // critical, time-sensitive code here
    // interrupts();

