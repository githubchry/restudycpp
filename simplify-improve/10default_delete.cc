#include <cstdio>
#include <iostream>

/*
在类的定义时，C++ 有一些规则决定是否生成默认的特殊成员函数。这些特殊成员函数可能包括：
    默认构造函数
    析构函数
    拷贝构造函数
    拷贝赋值函数
    移动构造函数
    移动赋值函数

生成这些特殊成员函数（或不生成）的规则比较复杂，感兴趣的话你可以查看[参考资料](https://zh.cppreference.com/w/cpp/language/member_functions)。

每个特殊成员函数有几种不同的状态：
    隐式声明还是用户声明
    默认提供还是用户提供
    正常状态还是删除状态
这三个状态是可组合的，虽然不是所有的组合都有效。隐式声明的必然是默认提供的；默认提供的才可能被删除；用户提供的也必然是用户声明的。

如果成员和父类没有特殊原因导致对象不可拷贝或移动，在用户不声明这些成员函数的情况下，编译器会自动产生这些成员函数，即隐式声明、默认提供、正常状态。
有特殊成员、用户声明的话，情况就非常复杂了：
    没有初始化的非静态 const 数据成员和引用类型数据成员会导致默认提供的默认构造函数被删除。
    非静态的 const 数据成员和引用类型数据成员会导致默认提供的拷贝构造函数、拷贝赋值函数、移动构造函数和移动赋值函数被删除。
    用户如果没有自己提供一个拷贝构造函数（必须形如 Obj(Obj&) 或 Obj(const Obj&)且不是模板），编译器会隐式声明一个。
    用户如果没有自己提供一个拷贝赋值函数（必须形如 Obj& operator=(Obj&) 或 Obj& operator=(const Obj&)且不是模板），编译器会隐式声明一个。
    用户如果自己声明了一个移动构造函数或移动赋值函数，则默认提供的拷贝构造函数和拷贝赋值函数被删除。
    用户如果没有自己声明拷贝构造函数、拷贝赋值函数、移动赋值函数和析构函数，编译器会隐式声明一个移动构造函数。
    用户如果没有自己声明拷贝构造函数、拷贝赋值函数、移动构造函数和析构函数，编译器会隐式声明一个移动赋值函数。
    ······
我们可以改变缺省行为，在编译器能默认提供特殊成员函数时将其删除，或在编译器不默认提供特殊成员函数时明确声明其需要默认提供。
（不过，要注意，即使用户要求默认提供，编译器也可能根据其他规则将特殊成员函数标为删除）
*/

// 对于下面这样的类，编译器看到有用户提供的构造函数，就会不默认提供默认构造函数：
template<typename T>
class my_array {
public:
    my_array(size_t size) : size_(size) {}

    //在没有默认初始化时，我们如果需要默认构造函数，就需要手工写一个，如：
    //my_array(): data_(nullptr), size_(0) {}
    //可有了默认初始化之后，这个构造函数显然就不必要了，所以我们现在可以写：
    my_array() = default;

private:
    T *data_{nullptr};
    size_t size_{0};
};

// 再看反例，只能指针ShapeWrapper的复制行为是不安全的
class Shape {
public:
    char *shapeBuf = nullptr;

    Shape() {
        shapeBuf = new char[256];
        std::cout << "new Shape buf" << std::endl;
    }

    virtual ~Shape() {
        std::cout << "delete Shape buf" << std::endl;
        delete[] shapeBuf;
    }
};

class ShapeWrapper {
public:
    explicit ShapeWrapper(Shape *ptr = nullptr) : ptr_(ptr) {}

    ~ShapeWrapper() { delete ptr_; }

    Shape *get() const { return ptr_; }

    // ShapeWrapper的复制行为是不安全的。如果正常情况不需要复制行为、只是想防止其他开发人员误操作时，我们可以简单地在类的定义中加入：
    ShapeWrapper(const ShapeWrapper &) = delete;
    ShapeWrapper &operator=(const ShapeWrapper &) = delete;
    /*
    在 C++11 之前，我们可能会用在 private 段里声明这些成员函数的方法，来达到相似的目的。
    但目前这个语法效果更好，可以产生更明确的错误信息。
    另外，你可以注意一下，用户声明成删除也是一种声明，因此编译器不会提供默认版本的移动构造和移动赋值函数。
    */
private:
    Shape *ptr_;
};


int main() {
    my_array<int> a();
    my_array<int> b(120);

    ShapeWrapper sw0(new Shape());

    // ShapeWrapper sw1(sw0);  // 编译错误：拷贝构造函数被delete
    // ShapeWrapper sw2 = sw0; // 编译错误：拷贝赋值函数被delete
    return 0;
}

