#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softTone.h>
#include "soft_tone.h"


typedef enum { false, true } bool;

int scale[23] = {659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 0, 0, 392, 0, 0, 0, 523, 0, 0, 392, 0, 0, 330};

static void play(int pin, int *music, int lenth){
	int i=0;
	for(i=0;i<lenth;i++){
		softToneWrite(pin, music[i]);
	}
}

/*设置播放某一段歌曲一次，指定播放引脚，播放音符，音符长度*/
extern int play_once(int pin, int *music, int lenth){
	if(softToneCreate(pin)==-1)
		return =-1;
	play(pin,music, lenth);
	softToneStop(pin);
	return 1;
}
/*设置循环播放某一段歌曲，指定播放引脚，播放音符，音符长度，播放次数，播放间隔*/
extern int play_loop(int pin, int *music, int lenth, int times, int interval){
	int i=0;
	if(softToneCreate(pin)==-1)
		return -1;

	for(i=0;i<times;i++){
		play(pin, music, lenth);
		delay(interval);
	}
	softToneStop(pin);
	return i;
}

/*
int main (int argc, char *argv[]) {
  const char* programName = argv[0];
  if(argc < 2) {
    fprintf(stderr, "Usage: %s gpio_port\n", programName);
    return -1;
  }

  int gpioPort = atoi(argv[1]);

  wiringPiSetup();

  if (0 != softToneCreate(gpioPort)) {
    printf("Create software tone failed!\n");
    return 1;
  }

  int i;
  int loopCount = 0;
  int arrayLength = sizeof(scale) / sizeof(int);
  while (true) {
    printf("Has played the music for %d times\n", loopCount);
    
    for (i = 0; i < arrayLength; ++i) {
      softToneWrite(gpioPort, scale[i]);
      delay(200);
    }

    delay(500);
    ++loopCount;
  }
}
*/
