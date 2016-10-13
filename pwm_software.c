#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * A program to control the brightness of LED through software PWM using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

int main (int argc,char* argv[])
{
  if (argc < 2) {
    printf("Usage example: ./pwm_software gpio_port\n");
    return 1;
  }
  int gpioPort = atoi(argv[1]);

  wiringPiSetup();

  softPwmCreate(gpioPort, 0, 100);  // setup software PWM to the specified port with initial value 0 & PWM range 100

  int value = 0;
  while(1) {
	  fprintf(stdout,"Please input pwm value: ");
	  fscanf(stdin, "%d", &value);
      softPwmWrite(gpioPort, value);
  }

  return 0;
}
