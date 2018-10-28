#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#include <wiringPi.h>
}
#endif
#include "callservo.h"
#include "msensor.h"
#include "mcomn.h"
#include "wheel.h"

int state=STOP;

static void showdis(double dis){
	char str[100]={0.0};
	sprintf(str,"dis=%.3lf", dis);
	
	if(dis<=0.5 and state==FORWARD){
		control_wheel(0, STOP);
	}
}

int main(){
	int wheel_pins[4]={0,1,2,3};
	int echo_pins[2]={4,5};
	char c=' ';

	wiringPiSetup();

	if(addpins(wheel_pins[0], wheel_pins[1],wheel_pins[2],wheel_pins[3])==-1){
		return -1;
	}

	echo_setup(echo_pins[0], echo_pins[1], showdis);
	echo_start();

	while(1){
		fscanf(stdin, "%c", &c);
		fprintf(stdout,"the char you intput is %c\n", c);
		switch(c){
			case 'w':
				state=FORWARD;
				break;
			case 's':
				state=BACKOFF;
				break;
			case 'a':
				state=TURNLEFT;
				break;
			case 'd':
				state=TURNRIGHT;
				break;
			case 'p':
				state=STOP;
				break;
			default:
				break;
		}
		control_wheel(0,state);
	}
	return 1;
}
