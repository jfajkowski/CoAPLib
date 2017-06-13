#ifndef COAPLIB_TEST_H
#define COAPLIB_TEST_H

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    #define beginTest
    #define endTest

    #include <ArduinoUnit.h>
    #include <CoAPLib.h>
#else
    #define beginTest int main() { cout << "Testing started!" << endl;
    #define test(x) cout << endl << "Testing: " << #x << endl << "----------------------------------------------------" << endl;
    #define endTest cout << endl << "Testing finished!" << endl; }
    #define assertEqual(x, y) assert(x == y)

    #include <functional>
    #include <cassert>
    #include <iostream>

    #include "../../src/CoAPLib.h"

    using namespace std;
#endif

#endif //COAPLIB_TEST_H
