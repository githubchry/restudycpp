/*
我们常常会说，C++ 里有左值和右值。这话不完全对。标准里的定义实际更复杂，规定了下面这些值类别（value categories）：

                   ┌────────────┐
                   │ expression │
                   └───╱────╲───┘
                      ╱      ╲
          ┌──────────╱─┐     ┌╲───────────┐
          │  glvalue   │     │   rvalue   │
          └───╱───╲────┘     └───╱───╲────┘
             ╱     ╲            ╱     ╲
┌───────────╱┐     ┌╲──────────╱┐     ┌╲───────────┐
│   lvalue   │     │   xvalue   │     │  prvalue   │
└────────────┘     └────────────┘     └────────────┘
                   C++表达式的值类别
 lvalue => left value 左值，可以放在等号左边的表达式
 rvalue => right value 右值，可以放在等号右边的表达式
 glvalue => generalized left value 广义左值
 xvalue => expiring lvalue 将亡值
 prvalue => pure rvalue 纯右值


我们平时概念上理解的左值可以是右值（int a = 0; int b = a），而右值实际上在这是prvalue纯右值，先从这两看起...

左值是有标识符、可以取地址的表达式，最常见的情况有：
    变量、函数或数据成员的名字。
    返回左值引用的表达式，如 ++x、x = 1、cout << ' '、字符串字面量如 "hello world"。
在函数调用时，左值可以绑定到左值引用的参数，如 T&。一个常量只能绑定到常左值引用，如 const T&。

纯右值 prvalue 是没有标识符、不可以取地址的表达式，一般也称之为“临时对象”。最常见的情况有：
    返回非引用类型的表达式，如 x++、x + 1、make_shared<int>(42)
    除字符串字面量之外的字面量，如 42、true

在C++11之前，右值可以绑定到常左值引用的参数，如const T&，但不可以绑定到非常左值引用，如T&。
从C++11开始，C++语言多了一种引用类型——右值引用。右值引用的形式是T&&。
**/

/**
 * @brief 演示`右值可以绑定到常左值引用的参数，但不可以绑定到非常左值引用`
 * @param a 常左值引用
 * @param b 非常左值引用
 *
 * 例：
 * int variable = 42;
 * left_ref_func(42, variable); // 正常运行: 右值42可以绑定到`常左值引用const T&`的参数
 * left_ref_func(variable, 42); // 编译错误: 右值42不可以绑定到`非常左值引用T&`的参数
 */
void left_ref_func(const int &a, int &b) {}

int main() {
    int variable = 42;
    const int constant = 42;

    // 左值引用只能和左值进行绑定
    int &left_ref = variable;           // 正常，左值引用和左值进行绑定
    //int    &err_ref1 = 10 + 1;        // 错误，10 + 1 是表达式，属于右值
    //string &err_ref2 = "test";        // 错误，"test" 是字符串字面值，属于右值
    //string &err_ref3 = string("test");// 错误，string("test") 是临时对象，属于右值

    // 右值可以绑定到常左值引用的参数，但不可以绑定到非常左值引用
    left_ref_func(constant, variable);
    left_ref_func(42, variable); // 正常运行: 右值42可以绑定到`常左值引用const T&`的参数
    // left_ref_func(variable, 42); // 编译错误: 右值42不可以绑定到`非常左值引用T&`的参数

    int *var_ref = &variable;
    const int *const_ref = &variable;

    return 0;
}