cmake_minimum_required(VERSION 3.3)
project(CommonLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        ioworker.cpp
        ioworker.h
        mylog.h)

add_executable(CommonLib ${SOURCE_FILES})