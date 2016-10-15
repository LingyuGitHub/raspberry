#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include "callservo.h"

static int cycle_time=20000;
static int step_size=10;
static int min=50;
static int max=250;
int p1_num=0, p5_num=0;
static int *p1pins=NULL;
static int *p5pins=NULL;

const static char *FIFO_FILE = "/dev/servoblaster";
/*board*/
/*const static int p1pins_default[8]={7,11,12,13,15,16,18,22};*/

/*wiringpi*/
const static int p1pins_default[8]={7, 0, 1, 2, 3, 4, 5, 6};	/*wiringpi*/
const static int p1pins_default_num=8;
const static int wiringpi2board[31]=
	{
		11,12,13,15,16,		/*0-4*/
		18,22, 7, 3, 5,		/*5-9*/
		24,26,19,21,23,		/*10-14*/
		 8,10,-1,-1,-1,		/*15-19*/
		-1,29,31,33,35,		/*20-24*/
		37,32,36,38,40,		/*25-30*/
		27					/*30*/
	};

/*设置脉冲循环时间为time us，默认20000us*/
extern void set_cycle_time(int time){
	cycle_time=time;
}
/*设置脉冲步长size us，默认10us*/
extern void set_step_size(int size){
	step_size=size;
}

/*设置最小控制脉冲时间 us，默认500us*/
extern void set_min(int min_){
	min=min_;
}
/*设置最大控制脉冲时间 us，默认2500us*/
extern void set_max(int max_){
	max=max_;
}
/*将wiringpi gpio编号转为板载编号*/
static int pi2board(int pi_gpio){
	if(pi_gpio>30){
		return -1;
	}

	return wiringpi2board[pi_gpio];
}
/*设置servoblaster p1的引脚，输入引脚采用wiringpi编号*/
extern int set_p1pins(const int *pins, int n){
	int i=0, wpi=0;
	p1_num=n;
	p1pins=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		if((wpi=pins[i])>30){
			return -1;
		}
		p1pins[i]=wiringpi2board[wpi];
	}
	return 0;
}
/*设置servoblaster p5的引脚，输入引脚采用wiringpi编号*/
extern int set_p5pins(const int *pins, int n){
	int i=0, wpi=0;
	p5_num=n;
	p5pins=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		if((wpi=pins[i])>30){
			return -1;
		}
		p5pins[i]=wiringpi2board[wpi];
	}
	return 0;
}
/*把引脚数组拼成字符串，[1,2,3,4]="1,2,3,4"*/
static void pins2str(const int *pins, int n, char *str){
	int i=0;	

	if(n==0){
		return;
	}

	sprintf(str,"%d", pins[0]);
	for(i=1;i<n;i++){
		sprintf(str, "%s,%d", str, pins[i]);
	}
	return;
}

/*根据设置的参数启动 servoblaster*/
extern int servo_setup(char* redirect){
	char cmd[500]={"servod"};
	char pinstr[200]={0.0};

	sprintf(cmd, "%s --cycle-time=%d", cmd, cycle_time);
	sprintf(cmd, "%s --step-size=%d", cmd, step_size);
	sprintf(cmd, "%s --min=%d", cmd, min);
	sprintf(cmd, "%s --max=%d", cmd, max);

	if(p1pins==NULL){
		set_p1pins(p1pins_default, p1pins_default_num);
	}

	pins2str(p1pins,p1_num,pinstr);	
	sprintf(cmd, "%s --p1pins=%s", cmd, pinstr);

	if(p5pins!=NULL){
		pins2str(p5pins,p5_num,pinstr);	
		sprintf(cmd, "%s --p5pins=%s", cmd, pinstr);
	}

	if(redirect!=NULL){
		sprintf(cmd, "%s >> %s", cmd, redirect);
	}

	printf("%s\n", cmd);
	system(cmd);
	return 1;
}

static int get_pinindex(int pin){
	int i=0;
	for(i=0;i<p1_num;i++){
		if(p1pins[i]==pin)
			return i;
	}

	return -1;

}

/*设置指定gpio 输出指定步长脉冲*/
extern void servo_set_step(int pi_gpio, int step){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%d > %s", get_pinindex(pin), step, FIFO_FILE);
	system(cmd);
}
/*设置指定gpio 输出指定时间脉冲*/
extern void servo_set_percent(int pi_gpio, int percent){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%d%% > %s", get_pinindex(pin), percent, FIFO_FILE);

	system(cmd);
}
/*设置指定gpio 输出指定比例脉冲*/
extern void servo_set_us(int pi_gpio, int us){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%dus > %s", get_pinindex(pin), us, FIFO_FILE);

	system(cmd);
}
/*设置指定gpio 增加指定步长脉冲*/
extern void servo_set_addstep(int pi_gpio, int addstep){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%+d > %s", get_pinindex(pin), addstep, FIFO_FILE);

	system(cmd);
}

extern void servo_set(int pi_gpio, int value, int type){
	switch (type){
		case SET_TYPE_STEP:
			servo_set_step(pi_gpio, value);
			break;
		case SET_TYPE_TIME:
			servo_set_us(pi_gpio, value);
			break;
		case SET_TYPE_PCNT :
			servo_set_percent(pi_gpio, value);
			break;
		case SET_TYPE_ADD:
			servo_set_addstep(pi_gpio,value);
			break;
		default:
			break;
	}
}












