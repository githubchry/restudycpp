/*
引用坍缩和完美转发
对于一个实际的类型 T，它的左值引用是 T&，右值引用是 T&&。那么：
    1. 是不是看到 T&，就一定是个左值引用？  —— 是
    2. 是不是看到 T&&，就一定是个右值引用？ —— 否

关键在于，在有模板的代码里，对于类型参数的推导结果可能是引用。
我们可以略过一些繁复的语法规则，要点是，对于这样的代码：

    template <typename T> func(T&&){}

如果传递过去的参数是左值，T 的推导结果是左值引用；
如果传递过去的参数是右值，T 的推导结果是参数的类型本身。
如果 T 是左值引用，那 T&& 的结果仍然是左值引用 —— 即 type& && 坍缩成了type&。
如果 T 是一个实际类型，那 T&& 的结果自然就是一个右值引用。


事实上，很多标准库里的函数，连目标的参数类型都不知道，但我们仍然需要能够保持参数的值类型：
    左值的仍然是左值，右值的仍然是右值。
这个功能在 C++ 标准库中已经提供了，叫 std::forward。
它和 std::move 一样都是利用引用坍缩机制来实现。
*/


#include <iostream>

using namespace std;


class Shape {
public:
    Shape() {}

    ~Shape() {}
};

void foo(const Shape &) {
    cout << "foo(const Shape&)" << endl;
}

void foo(const Shape &&) {
    cout << "foo(const Shape&&)" << endl;
}

void bar(const Shape &s) {
    cout << "bar(const Shape&)" << endl;
    foo(s);
}

void bar(const Shape &&s) {
    cout << "bar(const Shape&&)" << endl;
    // Shape &&s是右值引用，但传递给foo的s是左值 => 推导结果亦必须是左值 => Shape &&s 坍塌成 Shape &s
    foo(s);
}

void bar_move(const Shape &&s) {
    cout << "bar_move(const Shape&&)" << endl;
    // Shape &&s是右值引用，但传递给foo的s是左值 => 推导结果亦必须是左值 => Shape &&s 坍塌成 Shape &s => move => Shape &&s

    foo(move(s));
    // foo(static_cast<Shape&&>(s));
}

template<typename T>
void bar_ex(T &&s) {
    /**
     * 在 T 是模板参数时，T&& 的作用主要是保持值类别进行转发，它有个名字就叫“转发引用”（forwarding reference）。
     * 因为既可以是左值引用，也可以是右值引用，它也曾经被叫做“万能引用”（universal reference）。
     *
     * A& && => T=A&, T是一个左值引用 => T &&推导结果必须是左值引用 => 引用坍塌 => A&
     * A && => T=A, T是一个实际类型 => 推导为右值引用 => A &&
     */

    cout << "bar_ex(T&&)" << endl;

    // std::forward利用`引用坍缩`机制来实现`完美转发` => 左值的仍然是左值，右值的仍然是右值。
    foo(std::forward<T>(s));

}

int main() {
    {
        cout << "Shape => nrvo => Shape && => bar(Shape &&)"
                " => 传递给foo参数是左值 => Shape && 坍塌成 Shape & => foo(Shape &)" << endl;
        bar(Shape());
    }

    cout << "==================================================" << endl;

    {
        cout << "Shape => Shape & => bar(Shape &) => foo(Shape &)" << endl;
        Shape temp;
        bar(temp);
    }

    cout << "==================================================" << endl;

    {
        /**
         * 如果想在bar里面调用右值引用的那个 foo 的重载，bar我们必须写成：
         *      foo(std::move(s));
         * 或者
         *      foo(static_cast<Shape&&>(s));
         */

        cout << "Shape => nrvo => Shape && => bar(Shape &&)"
                " => 传递给move参数是左值 => Shape && 坍塌成 Shape & => move(Shape &) => foo(Shape&&)" << endl;

        bar_move(Shape());
    }

    cout << "==================================================" << endl;
    // 可如果两个 bar 的重载除了调用 foo 的方式不一样，其他都差不多的话，我们为什么要提供两个不同的 bar 呢？
    // 在bar_ex使用forward利用`引用坍塌机制`实现`完美转发`
    {
        cout << "Shape => nrvo => Shape && =>bar_ex(Shape &&) => 模板参数Shape：右值 => forward完美转发 => foo(T &&)" << endl;
        bar_ex(Shape());
    }

    cout << "==================================================" << endl;
    {
        cout << "Shape => Shape & => bar_ex(Shape& &&) => 模板参数Shape&：左值"
                " => Shape& && 坍缩成 Shape& => forward完美转发 => foo(T&)" << endl;
        Shape temp;
        bar_ex(temp);
    }
}