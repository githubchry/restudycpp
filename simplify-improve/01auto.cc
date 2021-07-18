
// 自动类型推断，顾名思义，就是编译器能够根据表达式的类型，自动决定变量的类型（从C++14 开始，还有函数的返回类型），不再需要程序员手工声明。

#include <vector>       // std::vector
#include <iostream>     // std::cout

//不使用自动类型推断时，如果容器类型未知的话，我们还需要加上 typename（注意此处const 引用还要求我们写 const_iterator 作为迭代器的类型）：
template<typename T>
void foo(const T &v) {
    std::cout << "重载foo打印容器：";
    for (typename T::const_iterator it = v.begin(), end = v.end(); it != end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// 如果 begin 返回的类型不是该类型的 const_iterator 嵌套类型的话，那实际上不用自动类型推断就没法表达了。
// 比如，如果我们的遍历函数要求支持 C 数组的话，不用自动类型推断的话，就只能使用两个不同的重载：
template<typename T, std::size_t N>
void foo(const T (&a)[N]) {
    typedef const T *ptr_t;
    std::cout << "重载foo打印数组：";
    for (ptr_t it = a, end = a + N; it != end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

//如果使用自动类型推断的话，再加上 C++11 提供的全局 begin 和 end 函数，上面的代码可以统一成：
template<typename T>
void auto_foo(const T &c) {
    std::cout << "auto_foo自动识别容器数组并打印：";
    // 使用依赖参数查找(ADL，Argument-dependent lookup)[https://www.cnblogs.com/zl1991/p/7718718.html]
    for (auto it = std::begin(c), ed = std::end(c); it != ed; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

/*
从这个例子可见，自动类型推断不仅降低了代码的啰嗦程度，也提高了代码的抽象性，使我们可以用更少的代码写出通用的功能。

auto 实际使用的规则类似于(函数模板参数的推导规则)[https://zh.cppreference.com/w/cpp/language/template_argument_deduction]。
当你写了一个含 auto 的表达式时，相当于把 auto 替换为模板参数的结果。举具体的例子：
    1. auto a = expr; 意味着用 expr 去匹配一个假想的 template <typename T> f(T) 函数模板，结果为值类型。
    2. const auto& a = expr; 意味着用 expr 去匹配一个假想的 template <typename T> f(const T&) 函数模板，结果为常左值引用类型。
    3. auto&& a = expr; 意味着用 expr 去匹配一个假想的 template <typename T> f(T&&) 函数模板，
       根据 value-move/05forwarding_reference.cc 中我们讨论过的转发引用和引用坍缩规则，结果是一个跟 expr 值类别相同的引用类型。

*/
int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    int a[] = {1, 2, 3, 4, 5};

    foo(v);
    foo(a);

    auto_foo(v);
    auto_foo(a);

    return 0;
}