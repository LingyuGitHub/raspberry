/*
 * @Author: lingyu 
 * @Date: 2018-11-15 20:13:50 
 * @Last Modified by:   lingyu 
 * @Last Modified time: 2018-11-15 20:13:50 
 */
#ifndef _L298N_H_
#define _L298N_H_


class L298N 
{
public:
    L298N()
    {

    }
    L298N(int pin1, int pin2, int pin3, int pin4, int enA=-1, int enB=-1)
    {
        m_pin1=pin1;
        m_pin2=pin2;
        m_pin3=pin3;
        m_pin4=pin4;
        m_enA=enA;
        m_enB=enB;
        pinsInit();
    }
    L298N(int *pins, int enA=-1, int enB=-1)
    {
        m_pin1=pins[0];
        m_pin2=pins[1];
        m_pin3=pins[2];
        m_pin4=pins[3];
        m_enA=enA;
        m_enB=enB;
        pinsInit();
    }
    ~L298N()
    {

    }

    void setPins(int pin1, int pin2, int pin3, int pin4)
    {
        m_pin1=pin1;
        m_pin2=pin2;
        m_pin3=pin3;
        m_pin4=pin4;
        pinsInit();
    }

    void setEnablePins(int enA, int enB)
    {
        m_enA=enA;
        m_enB=enB;
        pinsInit();
    }

    void goFore()
    {
        gpioWrite(1,0,1,0);
    }
    void goBack()
    {
        gpioWrite(0,1,0,1);
    }
    void turnLeft()
    {
        gpioWrite(1,0,0,1);
    }
    void turnRight()
    {
        gpioWrite(0,1,1,0);
    }
    void stop()
    {
        gpioWrite(0,0,0,0);
    }

protected:
    void gpioWrite(int val1, int val2, int val3, int val4)
    {
        digitalWrite(m_pin1, val1);
        digitalWrite(m_pin2, val2);
        digitalWrite(m_pin3, val3);
        digitalWrite(m_pin4, val4);
    }
    void pinsInit()
    {
        pinMode(m_pin1, OUTPUT);
        pinMode(m_pin2, OUTPUT);
        pinMode(m_pin3, OUTPUT);
        pinMode(m_pin4, OUTPUT);
        if (m_enA>=0)
            pinMode(m_enA, OUTPUT);
        if (m_enB>=0)
            pinMode(m_enB, OUTPUT);
    }

private:
    int m_pin1, m_pin2, m_pin3, m_pin4;
    int m_enA, m_enB;
};

#endif
