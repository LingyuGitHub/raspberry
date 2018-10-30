#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc <= 1) 
    {
        printf("Usage example: %s gpio_pin\n", argv[0]);
        return -1;
    }
    int pin= atoi(argv[1]);

    if (wiringPiSetup())
        return -1;

    pinMode(pin, OUTPUT);  

    int level = 0;
    while (1) 
    {
        level = (0 == level) ? 1 : 0;
        digitalWrite(pin, level); 
        sleep(1);
    }

    return 0;
}
