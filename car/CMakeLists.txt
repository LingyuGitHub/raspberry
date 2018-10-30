cmake_minimum_required(VERSION 3.7)
project(car)
set(CMAKE_BUILD_TYPE_DEBUG)

set(SRC_LIST main.cpp 
            msensor.c 
            callservo.c 
            getdistance.c 
            mcomn.c 
            wheel.c)

link_libraries(wiringPi pthread)

add_executable(car ${SRC_LIST})
