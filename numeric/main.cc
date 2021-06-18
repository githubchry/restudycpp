#include "factor.h"
#include <iostream>

int main() {

    std::cout << lcm(10000, 8000) << std::endl;
    std::cout << lcm(8000, 10000) << std::endl;

    std::cout << gcd(10000, 8000) << std::endl;
    std::cout << gcd(8000, 10000) << std::endl;

    return 0;
}