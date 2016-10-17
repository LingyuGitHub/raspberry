#ifndef _WHEEL_H_
#define _WHEEL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define STOP       0
#define FORWARD    1
#define BACKOFF    2
#define TURNLEFT   3
#define TURNRIGHT  4

extern int addpins(int p0, int p1, int p2, int p3);
extern int control_wheel(int pin, int action);


#ifdef __cplusplus
}
#endif



#endif
