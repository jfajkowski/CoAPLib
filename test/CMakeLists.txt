find_package(Boost REQUIRED unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(CoAPTest CoAPTest.cpp ../src/CoAP.h ../src/CoAP.cpp)
target_link_libraries(CoAPTest
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})