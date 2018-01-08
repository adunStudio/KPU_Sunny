cmake_minimum_required(VERSION 3.8)
project(Sunny_Core)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp sunny.h)

file(GLOB SUNNY_FILES
        maths/*.h
        maths/*.cpp
        utils/*.h
        utils/*.cpp)

add_executable(Sunny_Core ${SOURCE_FILES} ${SUNNY_FILES})