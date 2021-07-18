
#include <iostream>     // std::cout
#include <array>

// g++ 增加编译参数 => -std=c++17
// cmake 指定为 C++17 版本 => set(CMAKE_CXX_STANDARD 17)



int main() {
    // 如果你用过 pair 的话，一般都不会使用下面这种形式：
    std::pair<int, int> pr1{1, 42};

    //使用 make_pair 显然更容易一些：
    auto pr2 = std::make_pair(1, 42);

    // 这是因为函数模板有模板参数推导，使得调用者不必手工指定参数类型；但 C++17 之前的类模板却没有这个功能，也因而催生了像 make_pair 这样的工具函数。
    // 在进入了 C++17 的世界后，这类函数变得不必要了。现在我们可以直接写：
    std::pair pr3{1, 42};

    // 在初次见到 array 时，我觉得它的主要缺点就是不能像 C 数组一样自动从初始化列表来推断数组的大小了：
    int a1[] = {1, 2, 3};
    std::array<int, 3> a2{1, 2, 3}; // 啰嗦
    // std::array<int> a3{1, 2, 3}; // 编译错误

    // 这个问题在 C++17 里也是基本不存在的。虽然不能只提供一个模板参数，但你可以两个参数全都不写 🤣：
    std::array a{1, 2, 3};

    return 0;
}

// 这种自动推导机制，可以是编译器根据构造函数来自动生成：
template<typename T>
struct AutoObj {
    AutoObj(T value) {};
};

AutoObj autoObj1{std::string("hello")}; // 得到 AutoObj<string>
AutoObj autoObj2{"hello"};                 // 得到 AutoObj<const char*>

// 也可以是手工提供一个推导向导，达到自己需要的效果：
template<typename T>
struct ManualObj {
    ManualObj(T value) {};
};

ManualObj(const char *) -> ManualObj<std::string>;
ManualObj manualObj{"hello"};              // 得到 AutoObj<string>

