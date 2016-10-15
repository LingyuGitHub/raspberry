#include <stdio.h>
#include <stdlib.h>
#include "callservo.h"


int main(){
	int pins[1]={0};
	int value=50;
	set_p1pins(pins,1);
	servo_setup("/dev/null");
	while(1){
		/*printf("intput value: ");
		fscanf(stdin, "%d", &value);*/
		printf("ms=%d\n", value);
		servo_set_step(pins[0], value);
		sleep(1);
		value= value==250? 50:value+1;
	}
}
