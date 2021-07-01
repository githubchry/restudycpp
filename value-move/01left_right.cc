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

引入一种额外的引用类型当然增加了语言的复杂性，但也带来了很多优化的可能性。
由于C++ 有重载，我们就可以根据不同的引用类型，来选择不同的重载函数，来完成不同的行为。
如智能指针里面的移动构造函数(T&&)和拷贝构造函数(T&)

简单分析一下移动构造函数：
    SmartPtr(SmartPtr&& other) { ptr_ = other.release(); }

变量 other 算是左值还是右值呢？根据定义，other 是个变量的名字，变量有标识符、有地址，所以它还是一个左值——虽然它的类型是右值引用。
尤其重要的是，拿这个 other 去调用函数时，它匹配的也会是左值引用。也就是说，类型是右值引用的变量是一个左值！

再看一下下面的代码：
    smart_ptr<shape> ptr1{new circle()};
    smart_ptr<shape> ptr2 = std::move(ptr1);

std::move(ptr)的作用是把一个左值引用强制转换成一个右值引用，而并不改变其内容。
可以把 std::move(ptr1) 看作是一个有名字的右值。为了跟无名的纯右值 prvalue 相区别，C++ 里目前就把这种表达式叫做 xvalue。
跟左值 lvalue 不同，xvalue 仍然是不能取地址的——这点上，xvalue 和 prvalue 相同。
所以，xvalue 和 prvalue 都被归为右值 rvalue。

我们用下面的图来表示会更清楚一点：

  　　　　╭─────────╮
 　　　　 │ glvalue │
 　　　　 ├─────────┤
 不可移动 │ lvalue  │
 　　　　 ├─────────┼─────────┬────────╮
 　可移动 │ xvalue  │ prvalue │ rvalue │
 　　　　 └─────────┴─────────┴────────╯
           有标识符   无标识符

“值类别”（value category）和“值类型”（value type）是两个看似相似、却毫不相干的术语。
前者指的是上面这些左值、右值相关的概念，后者则是与引用类型（reference type）相对而言，表明一个变量是代表实际数值，还是引用另外一个数值。

在 C++ 里，所有的原生类型、枚举、结构、联合、类都代表值类型，只有引用（&）和指针（*）才是引用类型。
在 Java 里，数字等原生类型是值类型，类则属于引用类型。
在 Python 里，一切类型都是引用类型。
**/


#include <utility>
#include <iostream>

using namespace std;

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

    int aaa = move(10+1);

    cout<<aaa<<endl;

    return 0;
}