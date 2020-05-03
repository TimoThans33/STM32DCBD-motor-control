#define COMPILE_LOGGING_CODE_ROSSERIAL
#include "decode.h"

// initialize global variables
volatile float target_vel;
volatile float kp;
volatile float ki;
volatile float kd;
volatile uint32_t state;

// initialize local variables
char charbuf[8];

// initiate classes
ros::NodeHandle nh;
DigitalOut myled(LED1);
std_msgs::UInt32 int_msg;
ros::Publisher chatter2("cur_vel", &int_msg);

//-----------------------Function prototypes---------------------------------
void decodeCb(const std_msgs::String& str_msg);
//---------------------------------------------------------------------------
// initiate classes

ros::Subscriber<std_msgs::String> sub("par_val", &decodeCb);

void decode(){
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter2);

  while (1){
    //decodethread.signal_wait(0x1);
    int_msg.data = (uint32_t) 1/(cur_vel*1e-6);
    chatter2.publish( &int_msg );
    nh.spinOnce();
    wait_ms(10);
  }
}

void decodeCb(const std_msgs::String& str_msg){
   for(int i=0; i<8; i++){
        charbuf[i] = NULL;
   }
   //printf("%s\n",str_msg.data);
   charbuf[0] = {*str_msg.data};
   switch (charbuf[0]){
        case 'K':
        {
            charbuf[1] = str_msg.data[1];
            charbuf[2] = str_msg.data[2];
            charbuf[3] = str_msg.data[3];
            nh.loginfo((char*) charbuf);
            if (charbuf[1] == 'p'){
                kp = (float) (charbuf[2]-48)*10 + (charbuf[3]-48);
            }
            if (charbuf[1] == 'd'){
                kd = (float) (charbuf[2]-48)*10 + (charbuf[3]-48);
            }
            if (charbuf[1] == 'i'){
                ki = (float) (charbuf[2]-48)*10 + (charbuf[3]-48);
            }
            break;
            }
        case 'V':
        {
            charbuf[1] = str_msg.data[1];
            charbuf[2] = str_msg.data[2];
            charbuf[3] = str_msg.data[3];
            nh.loginfo((char*) charbuf);
            target_vel = (float) (charbuf[1]-48)*100 + (charbuf[2]-48)*10 + (charbuf[3]-48);
            break;
            }
        case 'S':
        {
            charbuf[1] = str_msg.data[1];
            nh.loginfo((char*) charbuf);
            state = (uint32_t) (charbuf[1]-48);
            break;
            }
   }
   //nh.loginfo((char*) charbuf[1]);
   //decodethread.signal_set(0x1);
   myled = !myled;
}