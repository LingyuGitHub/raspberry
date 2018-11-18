#include <iostream>
#ifdef __cplusplus
extern "C" {
#include <wiringPi.h>
}
#endif
#include "tcpserver.hpp"
#include "l298n.hpp"
#include "servo.hpp"
#include "softtone.hpp"
#include "echo.hpp"
#include "CJsonObject/CJsonObject.hpp"

using namespace std;

L298N wheel;
TcpServer server;

void setAction(int act, int val)
{
	switch(act)
	{
		case 0:
			wheel.stop();
		break;
		case 1:
			wheel.goFore();
		break;
		case 2:
			wheel.goBack();
		break;
		case 4:
			wheel.turnLeft();
		break;
		case 8:
			wheel.turnRight();
		break;
		default:
		break;
	}
}

void onrecv(const char *msg)
{
	neb::CJsonObject jobj(msg);
	int act=0, val=0;
	if (jobj.Get("act", act))
	{
		if (jobj.Get("value", val))
		{
			setAction(act, val);
		}
		else
		{
			cout<<"Error:"<<jobj.GetErrMsg()<<endl;
		}
	}
	else
	{
		cout<<"Error:"<<jobj.GetErrMsg()<<endl;
	}
}

void onlog(int state, const char *msg, ...)
{

}

void setSoftTone()
{
	int scale[] = {MI_M, MI_M, ZERO, 
					MI_M, ZERO, DO_M, 
					MI_M, ZERO, SO_M, 
					ZERO, ZERO, ZERO, 
					SO_L, ZERO, ZERO, 
					ZERO, DO_M, ZERO, 
					ZERO, SO_L, ZERO, ZERO, MI_L};
	int marry[] = {SO_L, DO_M, DO_M, 
					DO_M, DO_M, SO_L, 
					RI_M, XI_L, DO_M, 
					DO_M, SO_L, DO_M, 
					FA_M, FA_M, MI_M, 
					RI_M, DO_M, XI_L, 
					DO_M, RI_M, RI_M, 
					SO_L, DO_M, DO_M, 
					DO_M, DO_M, SO_L, 
					RI_M, XI_L, DO_M, 
					DO_M, SO_L, DO_M, 
					MI_M, SO_M, MI_M, 
					DO_L, LA_L, RI_M, 
					MI_M, DO_M, DO_M};

	SoftTone softtone(0);
	softtone.create();
	softtone.play(marry, sizeof(marry)/sizeof(int), 5);
}

void testServo()
{
	Servo servo(0, 0, 200);
	servo.create();
	while (1)
	{
		int v=rand()%20;
		v+=5;
		servo.setValue(v);
		sleep(1);
	}
}

void testEcho()
{
	Echo echo(0,1);
	while (1)
	{
		cout<<echo.getDistance()<<endl;
		sleep(1);
	}
}

int main(int argc, char **argv){
	if (argc<=1)
		return 1;
	int port=atoi(argv[1]);
	int wheel_pins[4]={0,1,2,3};

	wiringPiSetup();

	//wheel.setPins(0,1,2,3);

	//server.setPort(8000);
	//server.setOnRecv(onrecv);
	//server.setOnLog(onlog);
	//server.start();

	testEcho();

	while (1)
	{
		sleep(1);
	}

	return 1;
}
