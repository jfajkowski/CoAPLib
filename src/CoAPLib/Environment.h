#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#define DEBUG 0

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    #if DEBUG
        #define DEBUG(x) Serial.print(x);
    #endif

    #define PRINT(x) Serial.print(x)
    #define PRINTLN(x) Serial.println(x)

    #include <Arduino.h>
#else
    #if DEBUG
        #define PRINT_DEBUG(x) std::cout << x;
    #endif

    #define String string
    #define PRINT(x) std::cout << x
    #define PRINTLN(x) std::cout << x << std::endl

    #include <cstring>
    #include <string>
    #include <iostream>

    using namespace std;
#endif

#endif //ENVIRONMENT_H
