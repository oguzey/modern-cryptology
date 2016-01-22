cmake_minimum_required(VERSION 3.1)
project(modern_cryptology)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES
        CommonLib/ioworker.h
        CommonLib/ioworker.cpp
        CommonLib/mylog.h
        CommonLib/logger.h
        DifferentialCryptanalysisOfBlockCiphers/HeysCipher.cpp
        DifferentialCryptanalysisOfBlockCiphers/HeysCipher.h
        DifferentialCryptanalysisOfBlockCiphers/RoundKey.cpp
        DifferentialCryptanalysisOfBlockCiphers/RoundKey.h
        DifferentialCryptanalysisOfBlockCiphers/main.cpp)

add_executable(modern_cryptology ${SOURCE_FILES})