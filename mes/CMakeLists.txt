cmake_minimum_required(VERSION 3.2)
project(mes)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp GlobalData.cpp)
add_executable(mes ${SOURCE_FILES})