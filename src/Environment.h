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
    #include <chrono>

    using namespace std;

    static chrono::steady_clock::time_point begin_time = chrono::steady_clock::now();

    static unsigned long millis() {
        chrono::steady_clock::time_point now = chrono::steady_clock::now();

        return (unsigned long) chrono::duration_cast<chrono::milliseconds>(now-begin_time).count();
    }
#endif

#endif //ENVIRONMENT_H
