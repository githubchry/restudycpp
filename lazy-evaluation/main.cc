//
// Created by chenr on 2021/6/8.
//

#include "lazy.hpp"
#include <math.h>
#include <iostream>


int main() {
    auto pi = lazy<double>([]() {

        std::cout << "惰性求值，只打印一次" << std::endl;

        return acos(-1.0);
    });

    auto area = double(pi) * 5 * 5;
    auto perimeter = (double) pi * 2 * 5;
    std::cout << "pi= " << pi() << std::endl;



    auto a = lazy<int>([]() { return 1; });
    auto b = lazy<int>([]() { return 2; });
    auto c = a; // 实际上只是把 []() { return 1; } 这个 lambda 存给了变量 c
    std::cout << "a= " << a() << ", b= " << b() << ", c= " << c() << std::endl;
    // => a= 1, b= 2, c= 1
    c() = 5;    // 改的是惰性求出来的值
    std::cout << "a= " << a() << ", b= " << b() << ", c= " << c() << std::endl;
    // => a= 1, b= 2, c= 5
    b = c;  // 实际上只是把 []() { return 1; } 这个 lambda 存给了变量 b
    std::cout << "a= " << a() << ", b= " << b() << ", c= " << c() << std::endl;
    // => a= 1, b= 1, c= 5
    b() = c;
    std::cout << "a= " << a() << ", b= " << b() << ", c= " << c() << std::endl;
    // => a= 1, b= 5, c= 5






    return 0; // 结果：.....
}
