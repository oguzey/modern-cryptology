cmake_minimum_required(VERSION 3.2)
project(DifferentialCryptanalysisOfBlockCiphers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

set(SOURCE_FILES
    tests/test_round_key.cpp
    RoundKey.cpp
    RoundKey.h)

add_executable(DifferentialCryptanalysisOfBlockCiphers ${SOURCE_FILES})