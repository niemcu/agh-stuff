cmake_minimum_required(VERSION 3.2)
project(mes)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp GlobalData.cpp FEMGrid.cpp FEMGrid.h Element1D.cpp Element1D.h Node.cpp Node.h)
add_executable(mes ${SOURCE_FILES})