cmake_minimum_required(VERSION 3.7)
project(servo)
set(CMAKE_BUILD_TYPE DEBUG)

set(SRC_LIST servo.c)

link_libraries(wiringPi)

add_executable(servo ${SRC_LIST})

