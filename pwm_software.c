#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * A program to control the brightness of LED through software PWM using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

#define DO 2093
#define RE 2349
#define MI 2637
#define FA 2794
#define SO 3136
#define LA 3520
#define XI 3951
#define DO1 4186
#define RI1 4698

typedef struct{
	int freq;
	int t_ms;
}TONE;
:q

TONE star_notation[]=
{
	{DO,
void beep(int freq, int t_ms){
	int range=0;
	if(freq<2000||freq>5000) return;
	range=600000/freq;
	pwmSetRange(range);
	pwmWrite(1,range/2);
	if(t_ms>0){
		delay(t_ms);
	}
}

int main (int argc,char* argv[])
{
    int gpioPort = 1;

  	wiringPiSetup();
	pinMode(1, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);

	pwmSetClock(32);

	int value = 0;
	while(1) {
		
	}

	return 0;
}
