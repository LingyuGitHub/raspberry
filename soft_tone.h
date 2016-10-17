#ifndef _SOFT_TONE_H_
#define _SOFT_TONE_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int play_once(int pin, int *music, int lenth);
extern int play_loop(int pin, int *music, int lenth, int times, int interval);


#ifdef __cplusplus
}
#endif

#endif

