
#include <iostream>     // std::cout
#include <complex>      // 虚数打印
#include <thread>
#include <bitset>

/*
using namespace std 会同时引入 std 名空间和里面的内联名空间（inline namespace），包括了下面的字面量运算符所在的三个名空间：
    std::literals::complex_literals
    std::literals::chrono_literals
    std::literals::string_literals

在产品项目中，一般不会（也不应该）全局使用 using namespace std。
这种情况下，应当在使用到这些字面量的作用域里导入需要的名空间，以免发生冲突。在类似上面的例子里，就是在函数体的开头写：
*/
using namespace std::literals::complex_literals;
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;

// 要在自己的类里支持字面量也相当容易，唯一的限制是非标准的字面量后缀必须以下划线`_`打头。
// 比如，假如我们有下面的长度类：
struct length {
    double value;
    enum unit {
        metre,      // 米 m
        kilometre,  // 千米 km
        millimetre, // 毫米 mm
        centimetre, // 厘米 cm
        inch,       // 寸
        foot,       // 英尺
        yard,       // 码
        mile,       // 英里
    };
    static constexpr double factors[] = {
            1.0, 1000.0, 1e-3, 1e-2,
            0.0254, 0.3048, 0.9144, 1609.344
    };

    explicit length(double v, unit u = metre) {
        value = v * factors[u];
    }
};

length operator+(length lhs, length rhs) {
    return length(lhs.value + rhs.value);
}

/*
我们可以手写 length(1.0, length::metre) 这样的表达式，但估计大部分开发人员都不愿意这么做吧。
反过来，如果我们让开发人员这么写，大家应该还是基本乐意的：
    1.0_m + 10.0_cm
要允许上面这个表达式，我们只需要提供下面的运算符即可：
*/
length operator "" _m(long double v) {
    return length(v, length::metre);
}

length operator "" _cm(long double v) {
    return length(v, length::centimetre);
}


int main() {
    // 字面量（literal）是指在源代码中写出的固定常量，它们在 C++98 里只能是原生类型，如：
    auto s = "hello";   // 字符串字面量，类型是 const char[6]
    auto i = 123;       // 整数字面量，类型是 int
    auto d = 0.1;       // 浮点数字面量，类型是 double
    auto f = 0.2f;      // 浮点数字面量，类型是 float
    auto u = 123u;      // 无符号整数字面量，类型是 unsigned int
    auto l = 123l;      // 整数字面量，类型是 long
    auto ul = 123ul;    // 无符号长整数字面量，类型是 unsigned long

    // C++11 引入了自定义字面量，可以使用 operator"" 后缀 来将用户提供的字面量转换成实际的类型。
    // C++14 则在标准库中加入了不少标准字面量。下面这个程序展示了它们的用法：
    std::cout << "i * i = " << 1i * 1i << std::endl;  // 虚数生成，添加头文件#include <complex>则打印(-1,0)， 否则打印1
    std::cout << "Waiting for 500ms" << std::endl;
    std::this_thread::sleep_for(500ms);  // 500ms 时间字面量
    std::cout << "Hello world"s.substr(0, 5) << std::endl;    // basic_string 字面量

    // 自定义长度字面量
    auto len = 1.0_m + 10.0_cm;
    std::cout << len.value << std::endl;

    // 从 C++14 开始，我们对于二进制也有了直接的字面量：
    unsigned mask = 0b111000000;
    /*
     * 这在需要比特级操作等场合还是非常有用的。
     * 不过，遗憾的是， I/O streams 里只有 dec、hex、oct 三个操纵器（manipulator），而没有 bin，
     * 因而输出一个二进制数不能像十进制、十六进制、八进制那么直接。
     * 一个间接方式是使用 bitset，但调用者需要手工指定二进制位数：
     */
    std::cout << std::bitset<9>(mask) << std::endl;

    /* 数字分隔符
     * 数字长了之后，看清位数就变得麻烦了。有了二进制字面量，这个问题变得分外明显。
     * C++14 开始，允许在数字型字面量中任意添加 ' 来使其更可读。具体怎么添加，完全由程序员根据实际情况进行约定。某些常见的情况可能会是：
     *      十进制数字使用三位的分隔，对应英文习惯的 thousand、million 等单位。
     *      十进制数字使用四位的分隔，对应中文习惯的万、亿等单位。
     *      十六进制数字使用两位或四位的分隔，对应字节或双字节。
     *      二进制数字使用三位的分隔，对应文件系统的权限分组。
     *      等等。
     * 一些实际例子如下：
     */
    mask = 0b111'000'000;
    long r_earth_equatorial = 6'378'137;
    double pi = 3.14159'26535'89793;
    const unsigned magic = 0x44'42'47'4E;

    return 0;
}

