add_executable(CoAPTest CoAPTest.cpp UdpEmulator.h UdpEmulator.cpp ../src/Frame.cpp ../src/Frame.h ../src/Array.hpp ../src/Codes.h)
target_link_libraries(CoAPTest
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})