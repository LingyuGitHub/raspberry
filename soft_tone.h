#ifndef _SOFT_TONE_H_
#define _SOFT_TONE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DO_L	262
#define RI_L	294  
#define MI_L	330
#define FA_L	349
#define SO_L	392
#define LA_L	440
#define XI_L	494

#define DO_M	523
#define RI_M	587
#define MI_M	659
#define FA_M	698
#define SO_M	784
#define LA_M	880
#define XI_M	988

#define DO_H	1046
#define RI_H	1175
#define MI_H	1319
#define FA_H	1397
#define SO_H	1568
#define LA_H	1760
#define XI_H	1976

#define ZERO    0
extern int play_once(int pin, int *music, int lenth);
extern int play_loop(int pin, int *music, int lenth, int times, int interval);


#ifdef __cplusplus
}
#endif

#endif

