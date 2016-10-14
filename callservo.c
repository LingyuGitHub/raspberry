#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>


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

static void set_cycle_time(int time){
	cycle_time=time;
}

static void set_step_size(int size){
	step_size=size;
}

static void set_min(int min_){
	min=min_;
}

static void set_max(int max_){
	max=max_;
}

static int pi2board(int pi_gpio){
	if(pi_gpio>30){
		return -1;
	}

	return wiringpi2board[pi_gpio];
}
static int set_p1pins(const int *pins, int n){
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

static int set_p5pins(const int *pins, int n){
	int i=0, wpi=0;
	p5_num=n;
	p5pins=(int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		if((wpi=pins[i])>30){
			return -1;
		}
		p5pins[i]=pins[wpi];
	}
	return 0;
}

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

extern int servo_setup(){
	char cmd[500]={"servod"};
	char pinstr[200]={0.0};

	sprintf(cmd, "%s --cycle-time=%d", cmd, cycle_time);
	sprintf(cmd, "%s --step-size=%d", cmd, step_size);
	sprintf(cmd, "%s --min=%d", cmd, min);
	sprintf(cmd, "%s --max=%d", cmd, max);

	printf("%s\n", cmd);

	if(p1pins==NULL){
		set_p1pins(p1pins_default, p1pins_default_num);
	}

	pins2str(p1pins,p1_num,pinstr);	
	sprintf(cmd, "%s --p1pins=%s", cmd, pinstr);

	if(p5pins!=NULL){
		pins2str(p5pins,p5_num,pinstr);	
		sprintf(cmd, "%s --p5pins=%s", cmd, pinstr);
	}

	printf("%s\n", cmd);
	system(cmd);
	return 1;
}

extern void servo_setpwm(int pi_gpio, int step){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%d > %s", pin, step, FIFO_FILE);
	system(cmd);
}

extern void servo_setpwm_percent(int pi_gpio, int percent){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%d%% > %s", pin, percent, FIFO_FILE);

	system(cmd);
}

extern void servo_setpwm_us(int pi_gpio, int us){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%dus > %s", pin, us, FIFO_FILE);

	system(cmd);
}

extern void servo_setpwm_addstep(int pi_gpio, int addstep){
	int pin=pi2board(pi_gpio);
	char cmd[100]={0.0};

	sprintf(cmd, "echo %d=%+d > %s", pin, addstep, FIFO_FILE);

	system(cmd);
}


int main(){
	return servo_setup();
}











