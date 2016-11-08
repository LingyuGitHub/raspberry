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
static int interval_s=1, interval_us=0;
/*高电平持续时间, us*/
static int high_us=20;

static pthread_t echo_thread;

/*回调函数*/
typedef void(*callfunc)(double distance);
static callfunc echo_callfunc=NULL;
static int echo_flag=0;
static lock_t echo_lock;

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
	if(echo_flag!=0){
		return;
	}
	echo_flag=1;
	time_s=get_us();
	while(digitalRead(echo_pin)==1){};
	time_e=get_us();
	dis=get_distance(time_s, time_e);
	if(echo_callfunc!=NULL){
		echo_callfunc(dis);
	}
	echo_flag=0;
}


extern void echo_settime(int us, int ms){
	if(us<10) {
		high_us=10;
	}else if(us>50){
		high_us=50;
	}else{
		high_us=us;
	}
	interval=ms<1000? 1000:ms;
	interval_s=interval/1000;
	interval_us=interval%1000*1000;
}

/*设置控制和信号输入引脚，回调函数*/
extern int echo_setup(int trig_p, int echo_p, void (*function)(double)){
	trig_pin=trig_p;
	echo_pin=echo_p;
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

static void echo_loop(){
	while(1){
		digitalWrite(trig_pin, HIGH);
		delayMicroseconds(high_us);
		digitalWrite(trig_pin,LOW);
		delay(interval);
		//delayMicroseconds(interval*1000);
		/*sleep(interval_s);//线程挂起后无法唤醒，调用回调函数 
		usleep(interval_us);*/
	}

}

extern int echo_start(){
	int ret=0;
	ret=pthread_create(&echo_thread,NULL,(void*)echo_loop,NULL);
	return ret;
}

extern void echo_stop(){
	pthread_cancel(echo_thread);
	pthread_join(echo_thread, NULL);
	echo_thread=0;
	digitalWrite(trig_pin,LOW);
}


