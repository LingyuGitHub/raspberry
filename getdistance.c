#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "getdistance.h"

//控制输出
static int trigGpioPort = INVALID_GPIO_PORT;
//信号接收
static int echoGpioPort=INVALID_GPIO_PORT;
//回调函数
static callfunc echoCallFunc;

static long long get_us(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	//return ((long long)tv.tv_sec*1000+(long long)tv.tv_usec/1000);
	return ((long long)tv.tv_sec*1e6+(long long)tv.tv_usec);
}

static double get_distance(long long time_s, long long time_e){
	double dis=0.0;
	dis=(time_e-time_s)*340.0/2.0;
	dis/=1.0e6;
	return dis;

}
static void get_echo(){
	long long time_s=0, time_e=0;
	double dis=0.0;
	time_s=get_us();
	//fprintf(stdout,"start echo  %lld\n", time_s);
	while(digitalRead(echoGpioPort)==1);
	time_e=get_us();
	//fprintf(stdout,"end echo  %lld\n", time_e);
	dis=get_distance(time_s, time_e);
	//fprintf(stdout,"distance=%.4lf\n", dis);
	if(echoCallFunc!=NULL){
		echoCallFunc(dis);
	}
}

/*设置控制和信号输入引脚，回调函数*/
extern int echo_setup(int trig_port, int echo_port, void (*function)(double)){
	int flag=1;
	trigGpioPort=trig_port;
	echoGpioPort=echo_port;
	echoCallFunc=function;

	wiringPiSetup();
	
	// register the function to deal with the echo events
	wiringPiISR(echoGpioPort, INT_EDGE_RISING, &get_echo);
  
	pinMode(trigGpioPort, OUTPUT);
	return 1;
}

/*进行一次回声测距，并设置高电平持续时间，最少为10us，最长为50us*/
extern void echo_do(int us){
	digitalWrite(trigGpioPort,HIGH);
	if(us<10) us=10;
	if(us>50) us=50;
	delayMicroseconds(us); //at least 10us
	digitalWrite(trigGpioPort,LOW);
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

