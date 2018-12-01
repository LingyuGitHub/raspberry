#ifndef _COMM_HPP_
#define _COMM_HPP_
#include <string.h>

#define STOP 0x00
#define GOFORE 0x01
#define GOBACK 0x02
#define TURNLEFT 0x04
#define TURNRIGHT 0x08
#define VIEWACTION 0x10


void servoTurn(int angle);
double getDistance();
void danger();
void undanger();

String getmsg();
bool parseCMD(String cmd, int &act, int &val);
void setAction(int act, int val);

#endif