cmake_minimum_required(VERSION 3.3)
project(DifferentialCryptanalysisOfBlockCiphers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    HeysCipher.c
    HeysCipher.h
    main.cpp
    RoundKey.c
    RoundKey.h)

add_executable(DifferentialCryptanalysisOfBlockCiphers ${SOURCE_FILES})