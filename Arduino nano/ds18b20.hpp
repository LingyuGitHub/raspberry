/*
 * @Author: lingyu 
 * @Date: 2018-11-15 20:13:58 
 * @Last Modified by:   lingyu 
 * @Last Modified time: 2018-11-15 20:13:58 
 */
#ifndef _DS18B20_H_
#define _DS18B20_H_


class DS18B20
{
public:
    DS18B20(int pin)
    {
        m_pin=pin;
    }
    ~DS18B20()
    {

    }

    int getPin()
    {
        return m_pin;
    }

    void setPin(int pin)
    {
        if (pin>=0)
            m_pin=pin;
    }

    bool getTemprature(float &val)
    {
        if (m_pin<0)
            return false;
        if (!reset())
            return false;
        writeByte(0xcc); //忽略ROM
        writeByte(0x44); //开始温度变换
        if (!reset())
            return false;
        writeByte(0xcc);  //忽略ROM
        writeByte(0xbe);  //读暂存
        int lsb = readByte(); //读LSB
        int msb = readByte(); //读MSB
        val = convert(lsb, msb);
        return true;
    }

protected:
    bool reset()
    {
        int ack = false;
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, HIGH);
        digitalWrite(m_pin, LOW);
        delayMicroseconds(480);
        digitalWrite(m_pin, HIGH);
        delayMicroseconds(30);
        pinMode(m_pin, INPUT);
        pinMode(m_pin, INPUT_PULLUP);
        if (digitalRead(m_pin) == LOW)
            ack = true;
        else
            ack = false;
        delayMicroseconds(450);
        return ack;
    }
    void writeBit(int bit)
    {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(m_pin, bit);
        delayMicroseconds(30);
        digitalWrite(m_pin, HIGH);
        delayMicroseconds(1);
    }
    int readBit()
    {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, HIGH);
        digitalWrite(m_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(m_pin, HIGH);
        pinMode(m_pin, INPUT);
        pinMode(m_pin, INPUT_PULLUP);
        delayMicroseconds(2);
        int val= digitalRead(m_pin);
        delayMicroseconds(40);
        return val;
    }
    void writeByte(int byte)
    {
        for (int i = 0; i < 8; i++)
        {
            int val= byte & 0x01;
            writeBit(val);
            byte >>= 1;
        }
    }
    int readByte()
    {
        int val=0;
        for (int i = 0; i < 8; i++)
        {
            int tmp = readBit();
            val = (tmp << 7) | (val >> 1);
        }
        return val;
    }

    float convert(int lsb, int msb)
    {
        float temp;
        int val=0;

        if (msb >= 0xF0) //负温度
        {
            msb = (~msb) & 0XFF;
            lsb = ((~lsb) & 0XFF) + 1;
            val = -(msb << 8 | lsb);
        }
        else
        {
            val = (msb << 8) | lsb; //正温度
        }

        temp = val * 0.0625;
        return temp;
    }

private:
    int m_pin;
    float m_temp;

};

#endif