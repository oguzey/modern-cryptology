cmake_minimum_required(VERSION 3.2)
project(CommonLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        logger.h
        test_common_lib.cpp)

add_executable(tests_common_lib ${SOURCE_FILES})