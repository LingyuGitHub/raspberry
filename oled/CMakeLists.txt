cmake_minimum_required(VERSION 3.7)
project(oled)
set(CMAKE_BUILD_TYPE DEBUG)

set(SRC_LIST oled.cpp oled.h)

link_libraries(wiringPi ssd1306)

add_executable(oled ${SRC_LIST})

