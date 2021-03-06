


/*
函数对象及其特化

[显式（全）模板特化](https://zh.cppreference.com/w/cpp/language/template_specialization)

两个重要的函数对象，less 和 hash。

less，小于关系。在标准库里，通用的 less 大致是这样定义的：
    template<class T>
    struct less
            : binary_function<T, T, bool> {
        bool operator()(const T &x,
                        const T &y) const {
            return x < y;
        }
    };

也就是说，less 是一个函数对象，并且是个二元函数，执行对任意类型的值的比较，返回布尔类型。
作为函数对象，它定义了函数调用运算符（operator()），并且缺省行为是对指定类型的对象进行 < 的比较操作。

在需要大小比较的场合，C++ 通常默认会使用 less，包括我们今天会讲到的若干容器和排序算法 sort。
如果我们需要产生相反的顺序的话，则可以使用 greater，大于关系。


计算哈希值的函数对象 hash 就不一样了。它的目的是把一个某种类型的值转换成一个无符号整数哈希值，类型为 size_t。
它没有一个可用的默认实现。对于常用的类型，系统提供了需要的特化 [2]，类似于：
    template<class T>
    struct hash;

    template<>
    struct hash<int>
            : public unary_function<int, size_t> {
        size_t operator()(int v) const
        noexcept {
            return static_cast<size_t>(v);
        }
    };

这当然是一个极其简单的例子。更复杂的类型，如指针或者 string 的特化，都会更复杂。
要点是，对于每个类，类的作者都可以提供 hash 的特化，使得对于不同的对象值，函数调用运算符都能得到尽可能均匀分布的不同数值。
*/

#include <algorithm>    // std::sort
#include <functional>   // std::less/greater/hash
#include <iostream>     // std::cout/endl
#include <string>       // std::string
#include <vector>       // std::vector
#include "output_container.h"

using namespace std;

int main() {
    // 初始数组
    vector<int> v{13, 6, 4, 11, 29};
    cout << v << endl;

    // 从小到大排序
    sort(v.begin(), v.end()/*, less<int>()*/);  // 默认less
    cout << v << endl;

    // 从大到小排序
    sort(v.begin(), v.end(), greater<int>());
    cout << v << endl;

    cout << hex;
    auto hp = hash<int *>();
    cout << "hash(nullptr) = " << hp(nullptr) << endl;
    cout << "hash(v.data()) = " << hp(v.data()) << endl;
    cout << "v.data() = " << static_cast<void *>(v.data()) << endl;

    auto hs = hash<string>();
    cout << "hash(\"hello\") = " << hs(string("hello")) << endl;
    cout << "hash(\"hellp\") = " << hs(string("hellp")) << endl;

    // 可以看出，函数对象的值不重要。我们甚至可以认为，每个 less（或 greater 或 hash）对象都是等价的。
    // 关键在于其类型。以 sort 为例，第三个参数的类型确定了其排序行为。
    // 对于容器也是如此，函数对象的类型确定了容器的行为。
}