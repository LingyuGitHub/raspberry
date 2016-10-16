/*超声波测距模块，用户配置控制输出GPIO口，信号输入GPIO口，和测距结果回调函数*/
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "msensor.h"
#include "mcomn.h"

/*控制输出GPIO口*/
static int trip_port = INVALID_GPIO_PORT;
/*信号接收GPIO口*/
static int echo_port = INVALID_GPIO_PORT;

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
	while(digitalRead(echo_port)==1);
	time_e=get_us();
	dis=get_distance(time_s, time_e);
	if(echo_callfunc!=NULL){
		echo_callfunc(dis);
	}
}

/*设置控制和信号输入引脚，回调函数*/
extern int echo_setup(int trig_port_, int echo_port_, void (*function)(double)){
	int flag=1;
	trip_port=trig_port_;
	echo_port=echo_port_;
	echo_callfunc=function;

	wiringPiSetup();
	
	// register the function to deal with the echo events
	wiringPiISR(echo_port, INT_EDGE_RISING, &interruptfunc);
  
	pinMode(trip_port, OUTPUT);
	return 1;
}

/*进行一次回声测距，并设置高电平持续时间，最少为10us，最长为50us*/
extern void echo_do(int us){
	digitalWrite(trip_port,HIGH);
	if(us<10) us=10;
	if(us>50) us=50;
	delayMicroseconds(us); 
	digitalWrite(trip_port,LOW);
}

static void loop(){

}

extern int echo_start(){

	return 1;
}
extern int echo_stop(){

}




//demo
static void show(double dis){
	printf("distance=%.4lf\n",dis);
}

int main (int argc,char* argv[])
{
  if (argc < 3) {
    printf("Usage example: ./%s trig_gpio_port echo_gpio_port\n", argv[0]);
    return 1;
  }

	int p1 = atoi(argv[1]);
	int p2 = atoi(argv[2]);
	echo_setup(p1, p2, &show);

	while(true) {
		echo_do(20);	
		delay(2000);
	}

	return 0;
}

