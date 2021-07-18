
#include <iostream>     // std::cout
#include <vector>
#include <fstream>
#include <codecvt>
#include <locale>


/*
 * 类数据成员的默认初始化
 * 按照 C++98 的语法，数据成员可以在构造函数里进行初始化。
 * 这本身不是问题，但实践中，如果数据成员比较多、构造函数又有多个的话，逐个去初始化是个累赘，并且很容易在增加数据成员时漏掉在某个构造函数中进行初始化。
 * 为此，C++11 增加了一个语法，允许在声明数据成员时直接给予一个`初始化表达式`。
 * 这样，当且仅当构造函数的初始化列表中不包含该数据成员时，这个数据成员就会自动使用初始化表达式进行初始化。
 *
 * 例子：
 */
class ComplexRaw {
public:
    ComplexRaw() : re_(0), im_(0) {}

    ComplexRaw(float re) : re_(re), im_(0) {}

    ComplexRaw(float re, float im) : re_(re), im_(im) {}

private:
    float re_;
    float im_;
};

// 假设由于某种原因，我们不能使用缺省参数来简化构造函数，我们可以用什么方式来优化上面这个代码呢？
// 使用数据成员的默认初始化的话，我们就可以这么写：

class Complex {
public:
    // 第一个构造函数没有任何初始化列表，所以类数据成员的初始化全部由默认初始化完成，re_ 和 im_ 都是 0。
    Complex() {}

    // 第二个构造函数提供了 re_ 的初始化，im_ 仍由默认初始化完成。
    Complex(float re) : re_(re) {}

    // 第三个构造函数则完全不使用默认初始化。
    Complex(float re, float im) : re_(re), im_(im) {}

private:
    float re_{0};   // 默认初始化
    float im_{0};   // 默认初始化
};

int main() {
    ComplexRaw raw1{};
    ComplexRaw raw2{0};
    ComplexRaw raw3{0, 0};

    Complex c1{};
    Complex c2{0};
    Complex c3{0, 0};


    return 0;
}

