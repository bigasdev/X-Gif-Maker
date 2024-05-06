#pragma once

#include <iostream>

#ifdef F_ENABLE_ASSERTS
#define F_ASSERT(x) { \
    if(!(x)){ \
        std::cerr << "Assertion Failed: " << #x << std::endl;  \
        __debugbreak(); \
    }\
}
#define F_TEST(x) { std::cerr << "Test: " << #x << std::endl; }
#else
#define F_ASSERT(x)
#define F_TEST(x)
#endif