#ifndef _MSENSOR_H_
#define _MSENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { false, true } bool;
#define INVALID_GPIO_PORT  -1

#define HIGH 1
#define LOW 0


/*超声波测距模块函数*/
extern int echo_setup(int trig_port, int echo_port, void(*function)(double));
extern void echo_do(int us);
extern int echo_start();
extern int echo_stop();

/*红外避障模块函数*/
extern int avoidsensor_setup(int sensor_port, void(*function)(void));

#ifdef __cplusplus
}
#endif

#endif
