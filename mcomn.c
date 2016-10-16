#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "mcomn.h"

/*获取一天内的ms时间*/
extern long long get_ms(){
	struct timeval tv={0.0};
	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec*1e3 + (long long)tv.tv_usec/1e3);
}
/*获取一天内的us时间*/
extern long long get_us(){
	struct timeval tv={0.0};
	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec*1e6 + (long long)tv.tv_usec);
}

