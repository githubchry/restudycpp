#include <cstdio>
#include <iostream>

/*
override 和 final 是两个 C++11 引入的新说明符。
它们不是关键词，仅在出现在函数声明尾部时起作用，不影响我们使用这两个词作变量名等其他用途。
这两个说明符可以单个或组合使用，都是加在类成员函数声明的尾部。

override 显式声明了成员函数是一个虚函数且覆盖了基类中的该函数。
如果有 override 声明的函数不是虚函数，或基类中不存在这个虚函数，编译器会报告错误。
这个说明符的主要作用有两个：
    给开发人员更明确的提示，这个函数覆写了基类的成员函数；
    让编译器进行额外的检查，防止程序员由于拼写错误或代码改动没有让基类和派生类中的成员函数名称完全一致。


final 则声明了成员函数是一个虚函数，且该虚函数不可在派生类中被覆盖。如果有一点没有得到满足的话，编译器就会报错。
final 还有一个作用是标志某个类或结构不可被派生。同样，这时应将其放在被定义的类或结构名后面。
*/

class A {
public:
    virtual void foo();

    virtual void bar();

    void foobar();
};

class B : public A {
public:
    void foo() override;        // OK
    void bar() override final;  // OK
    // void foobar() override;  // 非虚函数不能 override
};

class C final : public B {
public:
    void foo() override;    // OK
    // void bar() override; // final 函数不可 override
};

// class D : public C { }; // 错误：final 类不可派生


int main() {
    return 0;
}

