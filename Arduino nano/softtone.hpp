/*
 * @Author: lingyu 
 * @Date: 2018-11-17 18:52:22 
 * @Last Modified by: lingyu
 * @Last Modified time: 2018-11-17 19:49:37
 */

#include <wiringPi.h>
#include <softTone.h>
#include <iostream>
#include <thread>

using namespace std;

#ifndef _SOFTTONE_H_
#define _SOFTTONE_H_

#define DO_L	262
#define RI_L	294  
#define MI_L	330
#define FA_L	349
#define SO_L	392
#define LA_L	440
#define XI_L	494

#define DO_M	523
#define RI_M	587
#define MI_M	659
#define FA_M	698
#define SO_M	784
#define LA_M	880
#define XI_M	988

#define DO_H	1046
#define RI_H	1175
#define MI_H	1319
#define FA_H	1397
#define SO_H	1568
#define LA_H	1760
#define XI_H	1976

#define ZERO    0

class SoftTone 
{
public:
    SoftTone()
    {
        m_pin=-1;
        m_valid=false;
        m_run=false;
        m_tones=NULL;
        m_toneLen=0;
        m_times=0;
    }

    SoftTone(int pin)
    {
        m_pin=pin;
        m_valid=false;
        m_run=false;
        m_tones=NULL;
        m_toneLen=0;
        m_times=0;
    }

    ~SoftTone()
    {
        if (m_run)
            stopPlay();
        if (m_valid)
            softToneStop(m_pin);
    }

    bool create(int pin=-1)
    {
        if (pin>=0)
            m_pin=pin;
        if (m_pin<0)
            return false;
        if (softToneCreate(m_pin)!=0)
            return false;
        m_valid=true;
    }

    int getPin()
    {
        return m_pin;
    }

    void play(const int *music, int len, int times=1)
    {
        if (!m_valid || music==NULL || len<=0 || times<=0)
            return;

        if (m_run)
            stopPlay();

        m_run=true;

        m_toneLen=len;
        m_tones=new int[m_toneLen];
        memcpy(m_tones, music, sizeof(int)*len);
        m_times=times;

        m_playThread=thread(&SoftTone::playTone, this);
    }

    void stopPlay(bool join=true)
    {
        if (!m_valid || !m_run)
            return;
        m_run=false;
        if (join)
            m_playThread.join();
        delete[] m_tones;
        m_toneLen=0;
        m_times=0;
    }

protected:
    void playTone()
    {
        for (int i=0; i<m_times && m_run; i++)
        {
            for (int j=0; j<m_toneLen && m_run; j++)
            {
                softToneWrite(m_pin, m_tones[j]);
                delay(150);
            }
            softToneWrite(m_pin, ZERO);
            delay(1000);
        }
        if (m_run)
            stopPlay(false);
    }

private:
    int m_pin;
    bool m_valid;

    int *m_tones;
    int m_toneLen;
    int m_times;
    bool m_run;
    thread m_playThread;
};

#endif