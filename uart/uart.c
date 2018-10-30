/*************************************************************************
    > File Name:      uart.c
    > Author:         lingy
    > Created Time:   Tue 08 Nov 2016 02:10:43 UTC
    > Description:
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>


int main()
{
    int fd, i = 0;
    int UartBuff[100] = { 0 };
    wiringPiSetup();
    if (fd = serialOpen("/dev/ttyAMA0", 9600) < 0)
    {
        fprintf(stdout, "serial err\n");
        return EXIT_FAILURE;
    }

    serialPutchar(fd, 'a');
    while (1)
    {
        while (1)
        {
            if ((UartBuff[0] = serialGetchar(fd)) != 0)
                fprintf(stdout, "%c\n", UartBuff[0]);
            else
                break;
        }
        for (i = 0; i < 10; i++)
            serialPutchar(fd, 'b');
    }
    return EXIT_SUCCESS;
}


