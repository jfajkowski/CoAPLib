add_executable(CoAPTest CoAPTest.cpp ${SOURCES} UdpEmulator.cpp UdpEmulator.h)
target_link_libraries(CoAPTest
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})