#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softTone.h>
#include "softtone.h"


static void play(int pin, int *music, int lenth)
{
    int i = 0;
    for (i = 0; i < lenth; i++)
    {
        softToneWrite(pin, music[i]);
        delay(150);
    }
    softToneWrite(pin, ZERO);
}

/*设置播放某一段歌曲一次，指定播放引脚，播放音符，音符长度*/
extern int play_once(int pin, int *music, int lenth)
{
    if (softToneCreate(pin) == -1)
        return -1;
    play(pin, music, lenth);
    softToneStop(pin);
    return 1;
}
/*设置循环播放某一段歌曲，指定播放引脚，播放音符，音符长度，播放次数，播放间隔*/
extern int play_loop(int pin, int *music, int lenth, int times, int interval)
{
    int i = 0;
    if (softToneCreate(pin) == -1)
        return -1;

    for (i = 0; i < times; i++)
    {
        play(pin, music, lenth);
        delay(interval);
    }
    softToneStop(pin);
    return i;
}


int scale[23] = { MI_M, MI_M, ZERO, MI_M, ZERO, DO_M, MI_M, ZERO, SO_M, ZERO, ZERO, ZERO, SO_L, ZERO, ZERO, ZERO, DO_M, ZERO, ZERO, SO_L, ZERO, ZERO, MI_L };

int marry[] = { SO_L, DO_M, DO_M, DO_M, DO_M, SO_L, RI_M, XI_L, DO_M,DO_M,SO_L,DO_M,FA_M,FA_M,MI_M,RI_M,DO_M,XI_L,DO_M,RI_M,RI_M, SO_L, DO_M, DO_M, DO_M, DO_M, SO_L, RI_M, XI_L, DO_M,DO_M,SO_L,DO_M,MI_M, SO_M, MI_M, DO_L, LA_L,RI_M, MI_M,DO_M,DO_M };

int main(int argc, char *argv[])
{
    const char* programName = argv[0];
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s gpio_port\n", programName);
        return -1;
    }

    int gpioPort = atoi(argv[1]);

    wiringPiSetup();

    play_loop(gpioPort, marry, sizeof(marry) / sizeof(int), 3, 1000);
    return 1;
}






