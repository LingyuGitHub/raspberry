/*
 * @Author: lingyu 
 * @Date: 2018-11-18 12:13:12 
 * @Last Modified by: lingyu
 * @Last Modified time: 2018-11-18 19:58:26
 */

#ifndef _ECHO_H_
#define _ECHO_H_

#include <wiringPi.h>
#include <iostream>
#include <sys/time.h>

using namespace std;

class Echo 
{
public:
    Echo(int trigPin, int echoPin)
    {
        m_trigPin=trigPin;
        m_echoPin=echoPin;
        pinMode(m_trigPin, OUTPUT);
        pinMode(m_echoPin, INPUT);
    }
    ~Echo()
    {

    }
    double getDistance()
    {
        timeval tv1, tv2;

        digitalWrite(m_trigPin, LOW);
        delayMicroseconds(2);

        digitalWrite(m_trigPin, HIGH);
        delayMicroseconds(20);
        digitalWrite(m_trigPin, LOW);

        while(digitalRead(m_echoPin)!=1){}
        gettimeofday(&tv1, NULL);

        while(digitalRead(m_echoPin)==1){}
        gettimeofday(&tv2, NULL);

        double start=tv1.tv_sec*1E6+ tv1.tv_usec;
        double end=tv2.tv_sec*1E6+ tv2.tv_usec;

        double dis=(end-start)/1E6*340.0/2.0;
        dis*=100.0; //m to cm
        return dis;
    } 

protected:

private:
    int m_trigPin;
    int m_echoPin;
};

#endif