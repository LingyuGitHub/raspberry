#include <iostream>
#ifdef __cplusplus
extern "C" {
#include <wiringPi.h>
}
#endif
#include "tcpserver.hpp"
#include "include/l298n.hpp"

using namespace std;


void onrecv(const char *msg)
{

}

int main(){
	int wheel_pins[4]={0,1,2,3};

	wiringPiSetup();

	L298N wheel(0,1,2,3);

	while (1)
	{
		wheel.goFore();
		delay(4000);
		wheel.turnLeft();
		delay(4000);
		wheel.turnRight();
		delay(4000);
		wheel.goBack();
		delay(4000);
	}

	return 1;
}
