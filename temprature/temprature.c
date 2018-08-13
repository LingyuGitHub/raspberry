#include <wiringPi.h>
#include <stdio.h>

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

void getTemprature(int pin)
{
    if (oneWireReset(pin))
    {
        oneWireSendComm(pin, 0xcc);//忽略ROM
        oneWireSendComm(pin, 0x44);//开始温度变换
    }
    if (oneWireReset(pin))
    {
        oneWireSendComm(pin, 0xcc);//忽略ROM
        oneWireSendComm(pin, 0xbe);//读暂存
        int a = oneWireReceive(pin);//读LSB
        int b = oneWireReceive(pin);//读MSB
        printf("%d %d %.3lf\n", a, b, tempchange(a, b));
    }
}

int main()
{
    wiringPiSetup();
    getTemprature(0);
    return 1;
}
