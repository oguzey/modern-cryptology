cmake_minimum_required(VERSION 3.3)
project(modern_cryptology)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        DifferentialCryptanalysisOfBlockCiphers/HeysCipher.c
        DifferentialCryptanalysisOfBlockCiphers/HeysCipher.h
        CommonLib/ioworker.cpp
        CommonLib/ioworker.h
        CommonLib/mylog.h
        DifferentialCryptanalysisOfBlockCiphers/main.cpp
        DifferentialCryptanalysisOfBlockCiphers/RoundKey.c
        DifferentialCryptanalysisOfBlockCiphers/RoundKey.h CommonLib/logger.cpp CommonLib/logger.h)

add_executable(modern_cryptology ${SOURCE_FILES})