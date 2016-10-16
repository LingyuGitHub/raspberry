/*通过L298N控制两个车轮的转向，实现前进、后退，左转、右转*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <string.h>

void mgpiowrite(int a0, int a1, int a2, int a3){
	digitalWrite(0, a0);
	digitalWrite(1, a1);
	digitalWrite(2, a2);
	digitalWrite(3, a3);
}

int main(int argc, char** argv){
	if(argc<2){
		fprintf(stdout, "Usage example: ./car gpio_port\n");
		return 1;
	}

	wiringPiSetup();
		
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);

	char cmd[10]={0.0};

	while(1){
		fprintf(stdout, "Please input cmd:  ", cmd);
		fscanf(stdin, "%s", cmd);
		
		if(strncmp(cmd, "up", 2)==0){
			mgpiowrite(1,0,1,0);
			continue;
		}
		if(strncmp(cmd, "back", 4)==0){
			mgpiowrite(0,1,0,1);
			continue;
		}
		if(strncmp(cmd, "left", 4)==0){
			mgpiowrite(0,1,1,0);
			continue;
		}
		if(strncmp(cmd, "right", 5)==0){
			mgpiowrite(1,0,0,1);
			continue;
		}
		if(strncmp(cmd,"stop", 4)==0){
			mgpiowrite(0,0,0,0);
			continue;
		}
		if(strncmp(cmd,"start", 5)==0){
			mgpiowrite(1,1,1,1);
			sleep(2);
			mgpiowrite(0,0,0,0);
			continue;
		}

	}
}
