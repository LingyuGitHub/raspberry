#ifndef _GETDISTANCE_H_
#define _GETDISTANCE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { false, true } bool;
#define INVALID_GPIO_PORT  -1
#define	DEBOUNCE_TIME      500

#define HIGH 1
#define LOW 0

typedef void(*callfunc)(double dis);

extern int echo_setup(int trig_port, int echo_port, void(*function)(double));
extern void echo_do(int us);
extern int echo_start();
extern int echo_stop();


#ifdef __cplusplus
}
#endif

#endif
