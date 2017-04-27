add_executable(CoAPTest CoAPTest.cpp ../src/CoAP.h ../src/CoAP.cpp UdpEmulator.h UdpEmulator.cpp)
target_link_libraries(CoAPTest
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})