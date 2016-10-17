#include <stdio.h>
#include <stdlib.h>
#include "callservo.h"
#include "msensor.h"
#include "mcomn.h"


int main(){
	int pins[1]={0};
	int value=0;
	servo_set_p1pins(pins,1);
	servo_setup("/dev/null");
	while(1){
		/*printf("intput value: ");
		fscanf(stdin, "%d", &value);*/
		fprintf(stdout,"degree=%d\n", value);
		servo_set_deg(pins[0], value);
		sleep(1);
		value= value==180? 0:value+1;
	}
}
