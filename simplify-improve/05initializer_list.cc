
#include <iostream>     // std::cout
#include <vector>

// [列表初始化](https://zh.cppreference.com/w/cpp/utility/initializer_list)

int main() {
    // 在 C++98 里，标准容器比起 C 风格数组至少有一个明显的劣势：不能在代码里方便地初始化容器的内容。比如，对于数组你可以写：
    int a[] = {1, 2, 3, 4, 5};

    // 而对于 vector 你却得写：
    std::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);

    // C++11 引入了列表初始化，允许以更简单的方式来初始化对象。现在我们初始化容器也可以和初始化数组一样简单了：
    std::vector<int> v2{1, 2, 3, 4, 5};

    /*
     * 同样重要的是，这不是对标准库容器的特殊魔法，而是一个通用的、可以用于各种类的方法。
     * 从技术角度，编译器的魔法只是对 {1, 2, 3} 这样的表达式自动生成一个初始化列表，在这个例子里其类型是 initializer_list<int>。
     * 程序员只需要声明一个接受 initializer_list 的构造函数即可使用。
     * 从效率的角度，至少在动态对象的情况下，容器和数组也并无二致，都是通过拷贝（构造）进行初始化。
     */

    return 0;
}

