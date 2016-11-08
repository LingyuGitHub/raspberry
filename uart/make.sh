#!/bin/bash
#########################################################################
# File Name:      make.sh
# Author:         lingy
# Created Time:   Tue 08 Nov 2016 03:29:47 UTC
# Description:    
#########################################################################


gcc -o serialRead serialRead.c -lwiringPi
gcc -o serialWrite serialWrite.c -lwiringPi -g3 -Wall -O0
