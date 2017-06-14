#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#define DEBUG 0
#define LIGHT_DEBUG 1

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    #include <Arduino.h>

    #if DEBUG
        #define DEBUG_PRINT_TIME() Serial.print("["); printTime(); Serial.print("] ")
        #define DEBUG_PRINT(x) Serial.print(x)
        #define DEBUG_PRINTLN(x) Serial.println(x)
        #define DEBUG_PRINTHEX(x) Serial.print(x, HEX)
        #define DEBUG_FUNCTION(x) x
    #else
        #define DEBUG_PRINT_TIME()
        #define DEBUG_PRINT(x)
        #define DEBUG_PRINTLN(x)
        #define DEBUG_PRINTHEX(x)

        #if LIGHT_DEBUG
            #define DEBUG_FUNCTION(x) x
        #else
            #define DEBUG_FUNCTION(x)
        #endif
    #endif

    #define substr substring
    #define TO_STRING(x) String(x)
    #define TO_INT(x) x.toInt()
    #define PRINT(x) Serial.print(x)
    #define PRINTLN(x) Serial.println(x)
#else
    #include <cstring>
    #include <string>
    #include <iostream>
    #include <chrono>
    #include <thread>
    #include <sstream>

    #if DEBUG
        #define DEBUG_PRINT_TIME() std::cout << "["; printTime(); std::cout << "] "
        #define DEBUG_PRINT(x) std::cout << x
        #define DEBUG_PRINTLN(x) std::cout << x << std::endl
        #define DEBUG_FUNCTION(x) x
    #else
        #define DEBUG_PRINT_TIME()
        #define DEBUG_PRINT(x)
        #define DEBUG_PRINTLN(x)

        #if LIGHT_DEBUG
            #define DEBUG_FUNCTION(x) x
        #else
            #define DEBUG_FUNCTION(x)
        #endif
    #endif

    #define String string
    #define TO_STRING(x) to_string(x)
    #define TO_INT(x) from_string<int>(x)
    #define PRINT(x) std::cout << x
    #define PRINTLN(x) std::cout << x << std::endl

    using namespace std;

    static chrono::steady_clock::time_point begin_time = chrono::steady_clock::now();

    static unsigned long millis() {
        chrono::steady_clock::time_point now = chrono::steady_clock::now();

        return (unsigned long) chrono::duration_cast<chrono::milliseconds>(now-begin_time).count();
    }

    template <typename T>
    std::string to_string(const T& n) {
        std::ostringstream stm;
        stm << n ;
        return stm.str() ;
    }

    template <typename T>
    T from_string(const String &s) {
        T result;
        stringstream(s) >> result;
        return result;
    }

    static void delay(unsigned long millis) {
        struct timespec tim, tim2;
        tim.tv_sec  = 0;
        tim.tv_nsec = 1000 * millis;
        nanosleep(&tim , &tim2);
    }

#endif


#if DEBUG
    static void printTime() {
        const long millis_in_second = 1000;
        const long millis_in_minute = millis_in_second * 60;
        const long millis_in_hour = millis_in_minute * 60;

        unsigned long millis_left = millis();

        unsigned short hours = (unsigned char) (millis_left / millis_in_hour);
        millis_left -= hours * millis_in_hour;
        unsigned short minutes = (unsigned char) (millis_left / millis_in_minute);
        millis_left -= minutes * millis_in_minute;
        unsigned short seconds = (unsigned char) (millis_left / millis_in_second);
        millis_left -= seconds * millis_in_second;

        PRINT(hours);
        PRINT(":");
        PRINT(minutes);
        PRINT(":");
        PRINT(seconds);
        PRINT(".");
        PRINT(millis_left);

    }
#endif

#endif //ENVIRONMENT_H
