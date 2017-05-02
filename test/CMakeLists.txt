add_executable(CoAPTest CoAPTest.cpp UdpEmulator.h UdpEmulator.cpp ../src/Frame.cpp ../src/Frame.h ../src/Array.hpp ../src/Constants.h ../src/Option.cpp ../src/Option.h Main.cpp OptionTest.cpp ArrayTest.cpp)
target_link_libraries(CoAPTest
        ${Boost_LIBRARIES})