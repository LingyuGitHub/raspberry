#include <iostream>
#ifdef __cplusplus
extern "C" {
#include <wiringPi.h>
}
#endif
#include "tcpserver.hpp"
#include "l298n.hpp"
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

int main(int argc, char **argv){
	if (argc<=1)
		return 1;
	int port=atoi(argv[1]);
	int wheel_pins[4]={0,1,2,3};

	wiringPiSetup();

	wheel.setPins(0,1,2,3);

	server.setPort(8000);
	server.setOnRecv(onrecv);
	server.setOnLog(onlog);
	server.start();

	while (1)
	{
	}

	return 1;
}
