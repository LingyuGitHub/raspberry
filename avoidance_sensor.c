/*红外避障传感器模块，配置信号输入引脚和中断触发函数*/
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "msensor.h"
#include "mcomn.h"


/*防止抖动*/
#define	DEBOUNCE_TIME      500

/*信号接收*/
static int sensor_gpio_port=INVALID_GPIO_PORT;
static long long time_1=0.0, time_2=0.0;

typedef void(*callfunc)(void);
/*回调函数*/
static callfunc sensor_callfunc;

static void interruputfunc(){
	time_2=get_ms();
	if((time_2-time_1)>DEBOUNCE_TIME){
		sensor_callfunc();
		time_1=time_2;
	}
}


/*设置控制和信号输入引脚，回调函数*/
extern int avoidsensor_setup(int sensor_port, void (*function)(void)){
	sensor_gpio_port=sensor_port;
	sensor_callfunc=function;

	/* register the function to deal with the echo events*/
	wiringPiISR(sensor_port, INT_EDGE_FALLING, interruputfunc);

	time_1=get_ms();
  
	return 1;
}


