/*超声波测距模块，用户配置控制输出GPIO口，信号输入GPIO口，和测距结果回调函数*/
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "msensor.h"
#include "mcomn.h"

/*控制输出GPIO口*/
static int trig_pin = INVALID_GPIO_PORT;
/*信号接收GPIO口*/
static int echo_pin = INVALID_GPIO_PORT;

/*loop 间隔 ms*/
static int interval=1000;
/*高电平持续时间, us*/
static int high_us=10;

static pthread_t echo_thread;

/*回调函数*/
typedef void(*callfunc)(double distance);
static callfunc echo_callfunc=NULL;

static double get_distance(long long time_s, long long time_e){
	double dis=0.0;
	dis=(time_e-time_s)*340.0/2.0;
	dis/=1.0e6;
	return dis;
}

/*在GPIO口上升沿触发，在下降沿退出，并统计之间的时间，计算距离，调用用户回调函数*/
static void interruptfunc(){
	long long time_s=0, time_e=0;
	double dis=0.0;
	time_s=get_us();
	while(digitalRead(echo_pin)==1);
	time_e=get_us();
	dis=get_distance(time_s, time_e);
	if(echo_callfunc!=NULL){
		echo_callfunc(dis);
	}
}

/*设置控制和信号输入引脚，回调函数*/
extern int echo_setup(int trig_pin_, int echo_pin_, void (*function)(double)){
	int flag=1;
	trig_pin=trig_pin_;
	echo_pin=echo_pin_;
	echo_callfunc=function;

	// register the function to deal with the echo events
	wiringPiISR(echo_pin, INT_EDGE_RISING, &interruptfunc);
  
	pinMode(trig_pin, OUTPUT);
	return 1;
}

/*进行一次回声测距，并设置高电平持续时间，最少为10us，最长为50us*/
extern void echo_do(int us){
	if(us<10) us=10;
	if(us>50) us=50;
	digitalWrite(trig_pin,HIGH);
	delayMicroseconds(us); 
	digitalWrite(trig_pin,LOW);
}



