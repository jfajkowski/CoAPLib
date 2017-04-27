cmake_minimum_required(VERSION 3.7)
project(ArduinoDemo)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_library(ArduinoDemo src/Frame.cpp src/Frame.h src/Array.hpp src/Codes.h)

add_subdirectory(test)