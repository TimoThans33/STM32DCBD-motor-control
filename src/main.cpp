#include "mbed.h"
#include "controller.h"
#include "decode.h"
//#include "message.h"

Thread controlthread(osPriorityNormal, 1024);
Thread decodethread(osPriorityNormal, 1024);
//Thread messagethread(osPriorityNormal, 1024);

int main(void)
{
  controlthread.start(control);
  decodethread.start(decode);
  while (1) {
  }
}
