#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    #define PRINT(x) Serial.print(x)
    #define PRINTLN(x) Serial.println(x)

    #include <Arduino.h>
#else
    #define String string
    #define PRINT(x) std::cout << x
    #define PRINTLN(x) std::cout << x << std::endl

    #include <cstring>
    #include <string>
    #include <iostream>

    using namespace std;
#endif

#endif //ENVIRONMENT_H
