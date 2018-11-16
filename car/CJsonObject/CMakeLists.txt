cmake_minimum_required(VERSION 3.7)
project(cjsonobject)
set(CMAKE_BUILD_TYPE_DEBUG)

set(SRC_LIST cJSON.c CJsonObject.cpp)

add_library(cjsonobject ${SRC_LIST})



