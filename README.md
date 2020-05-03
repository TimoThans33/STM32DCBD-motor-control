## STM32DCBD-motor-control
Real-time programming for a precise DC motor controller on a ARM Cortex M3/4 based microcontroller. It can recieve messages from ROS that control the motor and change the PD parameters of the speed control.
It also returns the current rpm of the motor to ROS. It is a great template that can be used for many future projects.
## Repo structure
```
├── mbed-os
├── src
     ├── NUCLEO_L432KC          # Includes the binary instructions
     ├── controller.cpp         # PD control loop
     ├── controller.h           # header files
     ├── decode.cpp             # decoding messages from ROS
     ├── main.cpp               # initialize RTOS for threading
     └── makefile               # makefile for ROS compatibility and gcc4mbed    
└── Readme.md
 ```
## Software
for compatibility with ROS make sure to install:
- gcc4mbed
- ROS melodic
