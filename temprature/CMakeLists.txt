cmake_minimum_required(VERSION 3.7)
project(temprature)
set(CMAKE_BUILD_TYPE DEBUG)

set(SRC_LIST temprature.c)

link_libraries(wiringPi)

add_executable(temprature ${SRC_LIST})

