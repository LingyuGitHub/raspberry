cmake_minimum_required(VERSION 3.7)
project(uart)
set(CMAKE_BUILD_TYPE DEBUG)

link_libraries(wiringPi)

add_executable(read serialRead.c)
add_executable(write serialWrite.c)
add_executable(demo uart.c)

