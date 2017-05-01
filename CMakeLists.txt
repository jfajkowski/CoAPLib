cmake_minimum_required(VERSION 3.7)
project(ArduinoDemo)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED unit_test_framework)
if(UNIX)
    add_definitions(-DBOOST_TEST_DYN_LINK)
endif()
include_directories(${Boost_INCLUDE_DIRS})

add_library(ArduinoDemo src/Frame.cpp src/Frame.h src/Array.hpp src/Codes.h src/Option.cpp src/Option.h)

add_subdirectory(test)