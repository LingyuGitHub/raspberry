cmake_minimum_required(VERSION 3.7)
project(main)
set(CMAKE_BUILD_TYPE_DEBUG)

set(SRC_LIST main.cpp)

link_libraries(wiringPi pthread cjsonobject)

add_executable(car ${SRC_LIST})
