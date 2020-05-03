#include "controller.h"

// Initialize global variables
volatile float speed;
volatile uint32_t cur_vel;
//volatile float target_vel;
//volatile int state;

// Initialize local variables
uint32_t motorstate[2];
volatile float p_error;
volatile float d_error;
volatile uint32_t i_error;
float buffer[6];
uint8_t rot_position;
int rot_count;
uint8_t first_time = 1;
uint8_t pointer;
float old_error;
bool DEBUG = false; //true for debug messages


// Initialize I/O pins
InterruptIn B(A0);
InterruptIn A(A1);

PwmOut PWM(A2);
DigitalOut ena_1(D4, 1);
DigitalOut ena_2(D9, 1);

//---------------------------Function Prototypes-----------------------
void driveISR(void);
void ISRHallSensor(void);
void motorHome(void);
void motorOut(void);
void measure_rotspeed(void);
void PID_output(void);
void controlticker(void);
//***********************************************************************

// Initialize (MBED) classes
Ticker t;
Timer speedtimer;

ros::NodeHandle nh;

// Start the main loop from this trhread
void control(){
  kp = 35;
  kd = 60;
  ki = 0;
  motorHome();
  ISRHallSensor();

  t.attach(&measure_rotspeed, 1.0);
  t.attach_us(&controlticker, 100);
  while(1){
    controlthread.signal_wait(0x1);
    // Check for division by zero
    if (cur_vel == 0.0){
      cur_vel = 1.0;
    }

    motorstate[0] = state;
    motorstate[1] = speed;
    motorOut();
    PID_output();
  }
}

void controlticker(){
  controlthread.signal_set(0x1);
}

void motorOut(){
  switch ((int) motorstate[0]){
    case (0):
      // Turn everything off
      ena_1 = 0;
      ena_2 = 0;
      break;
    case (1):
      // Turn motor forward at certain speed
      PWM.pulsewidth_us(motorstate[1]);
      ena_1 = 0;
      ena_2 = 1;
      break;
    case (2):
      // Turn motor backward at certain speed
      PWM.pulsewidth_us(motorstate[1]);
      ena_1 = 1;
      ena_2 = 0;
      break;

  }
}

void PID_output(){
    // Proportional error
    p_error = (float) target_vel - (float) (1.0/(cur_vel*1e-6));
    // Derivative error
    d_error = p_error - old_error;
    // Integral error
    i_error += (uint32_t) p_error;

    if (i_error < - 1000){
      i_error = -1000;
    }
    if (i_error > 1000){
      i_error = 1000;
    }

    old_error = p_error;
    //calculate the PWM output
    speed +=  p_error * kp*1e-3 + d_error * kd*1e-3 + i_error * ki*1e-6;
    if (speed > PWMLimit){
      speed = PWMLimit;
    }
    if (speed < 0){
      speed = 0.0;
    }
}

void motorHome(){
  //setMail(START,0);
  PWM.period_us(2000);
  PWM.pulsewidth_ms(0.8*PWMLimit);
  state = 1;
  speed = 0.8*PWMLimit;
  //motorOut();
  //ThisThread::sleep_for(10.0);
}

void ISRHallSensor(){
  B.rise(&driveISR);
  A.rise(&driveISR);
  B.fall(&driveISR);
  A.fall(&driveISR);
}

void driveISR(){
  rot_position++;
  // Check for one whole rotation
  if(rot_position == 12){
    speedtimer.stop();
    rot_position = 0;

    cur_vel = speedtimer.read_us();
    //controlthread.signal_set(0x1);
    speedtimer.reset();
    speedtimer.start();
  }
  if(first_time==1){
    target_vel = 250;//(float) 1.0/(cur_vel*1e-6);
    first_time = 0;
  }
  /*
  if (first_time == 1){
    target_vel = 1/(cur_vel*1e-6);
    first_time = 0;
  }
  */
}

void measure_rotspeed(){
  //setMail(SPEED1, (uint32_t) 1/(cur_vel*1e-6));
  //setMail(SPEED2, (uint32_t) speed);
  if (DEBUG == true){
    //setMail(DEBUG1, (uint32_t) (p_error*1000));
    //setMail(DEBUG3, (uint32_t) (d_error*1000));
    //setMail(DEBUG4, (uint32_t) i_error);
    //setMail(DEBUG2, (uint32_t) (target_vel*1000));
  }
}
