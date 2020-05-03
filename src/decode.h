#ifndef _decode_h_
#define _decode_h_

#include "mbed.h"
#include "rtos.h"
#include <ros.h>
#include <std_msgs/String.h>
#include "controller.h"


void decodeCb();

void decode();

extern volatile float target_vel;
extern volatile float kp;
extern volatile float kd;
extern volatile float ki;
extern volatile uint32_t state;

extern Thread decodethread;

#endif
