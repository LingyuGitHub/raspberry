#ifndef _CALL_SERVO_H_
#define _CALL_SERVO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SET_TYPE_STEP 0
#define SET_TYPE_TIME 1
#define SET_TYPE_PCNT 2
#define SET_TYPE_ADD  3

extern void servo_set_cycletime(int time);
extern void servo_set_stepsize(int size);
extern void servo_set_min(int min_);
extern void servo_set_max(int max_);	
extern int servo_set_p1pins(const int* pins, int n);
extern int servo_set_p5pins(const int* pins, int n);
extern int servo_setup(char* redirect);

extern void servo_set_step(int pi_gpio, int step);
extern void servo_set_us(int pi_gpio, int us);
extern void servo_set_percent(int pi_gpio, int percent);
extern void servo_set_addstep(int pi_gpio, int addstep);
extern void servo_set(int pi_gpio, int value, int type);
	

#ifdef __cplusplus
}
#endif


#endif

