/*************************************************************************
  > File Name:      servo.c
  > Author:         ly
  > Created Time:   Wed 31 May 2017 06:10:53 PM CST
  > Description:
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include <unistd.h>

static int s_pin=0;
static pthread_t s_pid=0;
static int s_angle=0;

extern void setServoAngle(int angle)
{
    s_angle=angle;
}

extern int getServoAngle()
{
    return s_angle;
}

static void *servoThread(void *arg)
{
    digitalWrite(s_pin, LOW);
    delayMicroseconds(10*2000);
    int h=0;
    while(1)
    {
        if (s_angle<0 || s_angle>180)
            continue;
        h=(s_angle+50)*10;
        digitalWrite(s_pin, HIGH);
        delayMicroseconds(h);
        digitalWrite(s_pin, LOW);
        delayMicroseconds(10*2000-h);
    }
}

extern int servoInit(int pin)
{
    if (s_pin<0)
        return -1;
    s_pin=pin;
    pinMode(s_pin, OUTPUT);
    pthread_create(&s_pid, NULL, servoThread, NULL);
}

int main(int argc, char *argv[]) 
{
    if (argc <= 1) 
    {
        printf("Usage example: %s gpio_pin\n", argv[0]);
        return -1;
    }

    int i = 0;
    int pin=atoi(argv[1]);
    if (wiringPiSetup())
        return -1;

    pinMode(pin, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);

    int v1=atoi(argv[2]);

    pwmWrite(pin, v1);
    delay(3000);
    //return -1;
    int v=0;
    while (1)
    {
        v=rand()%200;
        v+=50;
        printf("%d\n", v);
        pwmWrite(pin, v);
        delay(1000);
    }
    return 0;
}
