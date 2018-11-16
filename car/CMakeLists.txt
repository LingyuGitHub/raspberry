cmake_minimum_required(VERSION 3.7)
project(car)
set(CMAKE_BUILD_TYPE_DEBUG)

add_subdirectory(CJsonObject)

include_directories(include)
set(SRC_LIST main.cpp)

link_libraries(wiringPi pthread cjsonobject)

add_executable(car ${SRC_LIST})
