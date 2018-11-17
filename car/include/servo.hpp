/*
 * @Author: lingyu 
 * @Date: 2018-11-17 18:30:40 
 * @Last Modified by: lingyu
 * @Last Modified time: 2018-11-17 20:10:15
 */

#ifndef _SERVO_H_
#define _SERVO_H_

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

using namespace std;

class Servo
{
public:
    Servo()
    {
        m_pin=-1;
        m_value=-1;
        m_range=-1;
        m_valid=false;
    }
    Servo(int pin, int val, int range)
    {
        m_pin=pin;
        m_value=val;
        m_range=range;
        m_valid=false;
    }
    ~Servo()
    {

    }

    bool create(int pin=-1, int val=-1, int range=-1)
    {
        if (pin>=0 && val>=0 && range>0)
        {
            m_pin=pin;
            m_value=val;
            m_range=range;
        }

        if (m_pin<0 || m_value<0 || m_range<=0)
            return false;
        if (softPwmCreate(m_pin, m_value, m_range)!=0)
            return false;
        m_valid=true;
        return true;
    }

    void setValue(int val)
    {
        if (!m_valid || val<0 || val>m_range)
            return;
        m_value=val;
        softPwmWrite(m_pin, m_value);
    }

    int getValue()
    {
        return m_value;
    }

    bool getValid()
    {
        return m_valid;
    }
protected:

private:
    int m_pin;
    int m_range;
    int m_value;
    bool m_valid;
};

#endif