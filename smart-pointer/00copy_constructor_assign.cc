#include <iostream>

class Shape {
public:
    Shape() { std::cout << "构造函数Constructor" << std::endl; }

    Shape(const Shape &shape) { std::cout << "拷贝构造Copy Constructor" << std::endl; }

    Shape &operator=(const Shape &shape) {
        std::cout << "拷贝赋值Copy Assign" << std::endl;
        return *this;
    }

    ~Shape() { std::cout << "析构函数Destructor" << std::endl; }
};

int main() {
    Shape p1;  // 构造函数
    Shape p2;  // 构造函数
    Shape p3 = p1;  // 拷贝构造

    p2 = p1;  // 拷贝赋值

    [](Shape p) { return; }(p2);  // 拷贝构造：函数参数传递

    std::cout << "------重点来了------" << std::endl;
    auto func = []() {
        Shape p;   // 构造函数
        return p;  // 拷贝赋值: 函数返回值传递
    };

    p2 = func();  // 构造函数+拷贝赋值
    std::cout << "------重点又来了------" << std::endl;

    Shape p4 = func();  // 构造函数
    // 理论上应该是：构造函数+拷贝构造
    // 实际上是编译器优化了流程，直接调用构造函数使用返回值创建了对象p4。

    std::cout << "------完事------" << std::endl;

    return 0;
}
