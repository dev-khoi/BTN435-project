#include <iostream>
#include <cassert>
#include "../../simulation/devices.h"
 
int passed = 0;
int failed = 0;
 
#define TEST(name, expr) \
    do { \
        if (expr) { std::cout << "  [PASS] " << name << "\n"; passed++; } \
        else      { std::cout << "  [FAIL] " << name << "\n"; failed++; } \
    } while(0)