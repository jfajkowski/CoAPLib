add_executable(CoAPTest CoAPTest.cpp UdpEmulator.h UdpEmulator.cpp ../src/Frame.cpp ../src/Frame.h ../src/Array.hpp ../src/Codes.h ../src/Option.cpp ../src/Option.h)
target_link_libraries(CoAPTest
        ${Boost_LIBRARIES})