/*通过L298N控制两个车轮的转向，实现前进、后退，左转、右转*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>

#include "wheel.h"

#define MAX_WHEELS   4
static int pins[MAX_WHEELS][4]={{0.0}};
static int pin_num=0;

static void gpiow(int *ps, int a0, int a1, int a2, int a3){	
	digitalWrite(ps[0], a0);
	digitalWrite(ps[1], a1);
	digitalWrite(ps[2], a2);
	digitalWrite(ps[3], a3);
}
static int checkpins(int p0, int p1, int p2, int p3){
	int m=0,n=0, i=0;

	for(n=0;n<pin_num;n++){
		if(pins[n][0]!=p0 && pins[n][1]!=p0 && pins[n][2]!=p0 && pins[n][3]!=p0 &&
			pins[n][0]!=p1 && pins[n][1]!=p1 && pins[n][2]!=p1 && pins[n][3]!=p1 &&
			pins[n][0]!=p2 && pins[n][1]!=p2 && pins[n][2]!=p2 && pins[n][3]!=p2 &&
			pins[n][0]!=p3 && pins[n][1]!=p3 && pins[n][2]!=p3 && pins[n][3]!=p3 
			){
			continue;
		}else{
			return -1;
		}
	}
	return 1;
}

/*添加四个引脚，并检测该四个引脚是否被使用过，如果添加成功，返回驱动器个数*/
/*L1 L2  R1 R2*/
extern int addpins(int p0, int p1, int p2, int p3){
	if(checkpins(p0,p1,p2,p3)==-1){
		return -1;
	}

	if(pin_num==MAX_WHEELS){
		return -1;
	}
	
	pinMode(p0, OUTPUT);
	pinMode(p1, OUTPUT);
	pinMode(p2, OUTPUT);
	pinMode(p3, OUTPUT);

	pins[pin_num][0]=p0;
	pins[pin_num][1]=p1;
	pins[pin_num][2]=p2;
	pins[pin_num][3]=p3;
	pin_num+=1;
	return pin_num-1;
	/*return pin_num++;*/
}


extern int control_wheel(int group, int action){	
	if(group>=pin_num){
		return -1;
	}
	switch(action){
		case FORWARD:
			gpiow(pins[group], 1,0,1,0);	
			break;
		case BACKOFF:
			gpiow(pins[group], 0,1,0,1);	
			break;
		case TURNLEFT:
			gpiow(pins[group], 1,0,0,1);	
			break;
		case TURNRIGHT:
			gpiow(pins[group], 0,1,1,0);	
			break;
		case STOP:
			gpiow(pins[group], 0,0,0,0);	
			break;
		default:
			return -1;
	}

	return 1;

}

		
