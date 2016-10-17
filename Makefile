#*********************************************************************
# This is the makefile for the Adafruit SSD1306 OLED library driver
#
#	02/18/2013 	Charles-Henri Hallard (http://hallard.me)
#							Modified for compiling and use on Raspberry ArduiPi Board
#							LCD size and connection are now passed as arguments on 
#							the command line (no more #define on compilation needed)
#							ArduiPi project documentation http://hallard.me/arduipi
#
# *********************************************************************


# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Wall -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
CLIBS=-lpthread -lssd1306 -lwiringPi
OBJS=main.o callservo.o getdistance.o mcomn.o ssd1306.o wheel.o
#CCFLAGS=

# define all programs
BIN = mycar

all: ${BIN}

$(BIN): $(OBJS)
	g++ ${CCFLAGS} ${CLIBS} ${OBJS} -o $@


main.o: main.cpp mcomn.h msensor.h callservo.h wheel.h
	gcc -c $< -o $@
callservo.o: callservo.c callservo.h mcomn.h
	gcc -c $< -o $@
mcomn.o: mcomn.c mcomn.h
	gcc -c $< -o $@
msensor.: msensor.c msensor.h
	gcc -c $< -o $@
getdistance.o: getdistance.c 
	gcc -c $< -o $@
ssd1306.o: ssd1306.cpp ssd1306.h
	g++ -c $< -o $@ 
wheel.o: wheel.c wheel.h
	gcc -c $< -o $@

clean:
	rm -rf $(OBJS) $(BIN)



