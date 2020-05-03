#ifndef _controller_h_
#define _controller_h_

#include "mbed.h"
#include "rtos.h"
#include <ros.h>
#include <std_msgs/UInt32.h>
#include "decode.h"
//#include "message.h"

#define PWMLimit 2000 // This depends on the length of your period right?

extern volatile uint32_t cur_vel;

void driveISR();

void ISRHallSensor();

void motorHome();

void motorOut();

void measure_rotspeed();

void PID_output();

void control();

extern Thread controlthread;

#endif
