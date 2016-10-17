#ifndef _MCOMN_H_
#define _MCOMN_H_

#include <pthread.h>

typedef enum { false, true } bool;

#ifdef __cplusplus
extern "C" {
#endif

#define lock_t            pthread_mutex_t
#define initlock(f)       pthread_mutex_init(f,NULL)
#define lock(f)           pthread_mutex_lock(f)
#define unlock(f)         pthread_mutex_unlock(f)

extern long long get_ms();
extern long long get_us();


#ifdef __cplusplus
}
#endif

#endif
