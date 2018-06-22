cmake_minimum_required(VERSION 3.7)
project(blink)
set(CMAKE_BUILD_TYPE DEBUG)

set(SRC_LIST blink.c)

link_libraries(wiringPi)

add_executable(blink ${SRC_LIST})

