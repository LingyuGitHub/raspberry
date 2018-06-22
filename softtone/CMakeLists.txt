cmake_minimum_required(VERSION 3.7)
project(softtone)
set(CMAKE_BUILD_TYPE DEBUG)

set(SRC_LIST softtone.c softtone.h)

link_libraries(wiringPi)

add_executable(softtone ${SRC_LIST})

