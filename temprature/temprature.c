#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif

bool oneWireReset(int pin)
{
    int ack = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
    delayMicroseconds(480);
    digitalWrite(pin, HIGH);
    delayMicroseconds(30);
    pinMode(pin, INPUT);
    pullUpDnControl(pin, PUD_UP);
    if (digitalRead(pin) == LOW)
        ack = true;
    else
        ack = false;
    delayMicroseconds(450);
    return ack;
}

void writeBit(int pin, int bit)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pin, bit);
    delayMicroseconds(30);
    digitalWrite(pin, HIGH);
    delayMicroseconds(1);
}

void oneWireSendComm(int pin, int byte)
{
    for (int i = 0; i < 8; i++)
    {
        int sta = byte & 0x01;
        writeBit(pin, sta);
        byte >>= 1;
    }
}

int readBit(int pin)
{
    int tmp;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pin, HIGH);
    pinMode(pin, INPUT);
    pullUpDnControl(pin, PUD_UP);
    delayMicroseconds(2);
    tmp = digitalRead(pin);
    delayMicroseconds(40);
    return tmp;
}

int oneWireReceive(int pin)
{
    int i = 0, j = 0, k = 0;
    for (i = 0; i < 8; i++)
    {
        j = readBit(pin);
        k = (j << 7) | (k >> 1);
    }
    return k;
}

float tempchange(int lsb, int msb)
{
    float temp;
    int tem;

    if (msb >= 0xF0) //负温度
    {
        msb = (~msb) & 0XFF;
        lsb = ((~lsb) & 0XFF) + 1;
        tem = -(msb << 8 | lsb);
    }
    else
    {
        tem = (msb << 8) | lsb; //正温度
    }

    temp = tem * 0.0625;
    return temp;
}

bool getTemprature(int pin, double *t)
{
    if (!oneWireReset(pin))
        return false;
    oneWireSendComm(pin, 0xcc); //忽略ROM
    oneWireSendComm(pin, 0x44); //开始温度变换
    if (!oneWireReset(pin))
        return false;
    oneWireSendComm(pin, 0xcc);  //忽略ROM
    oneWireSendComm(pin, 0xbe);  //读暂存
    int a = oneWireReceive(pin); //读LSB
    int b = oneWireReceive(pin); //读MSB
    if (t != NULL)
        *t = tempchange(a, b);
    return true;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Usage example: %s gpio_port\n", argv[0]);
        return -1;
    }
    int pin=atoi(argv[1]);

    if (wiringPiSetup())
        return -1;

    double t = 0.0;
    while (1)
    {
        if (getTemprature(pin, &t))
            printf("Tempreture=%.1lf\n", t);
        sleep(2);
    }
    return 0;
}
