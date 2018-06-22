/*************************************************************************
  > File Name:      servo.c
  > Author:         ly
  > Created Time:   Wed 31 May 2017 06:10:53 PM CST
  > Description:
 ************************************************************************/

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

int main() 
{
    int i = 0;
    if (wiringPiSetup())
    {
        printf("error\n");
        return -1;
    }
    softPwmCreate(0, 0, 200);

    while (1)
    {
        for (i = 5; i <= 25; i++) 
        {
            softPwmWrite(0, i);
            printf("%d\n", i);
            delay(100);
        }
        for (i = 25; i >= 5; i--)
        {
            softPwmWrite(0, i);
            printf("%d\n", i);
            delay(100);
        }
    }
}
