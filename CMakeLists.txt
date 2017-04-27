cmake_minimum_required(VERSION 3.7)
project(ArduinoDemo)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES src/CoAP.cpp src/CoAP.h)
add_library(ArduinoDemo ${SOURCE_FILES})

add_subdirectory(test)