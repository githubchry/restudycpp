/*
不要返回`本地变量的引用`
有一种常见的 C++ 编程错误，是在函数里返回一个本地对象的引用。错误示例：
    A* func() { A a; return &a;}
由于在函数结束时本地对象即被销毁，返回一个指向本地对象的引用属于未定义行为。
理论上来说，程序出任何奇怪的行为都是正常的。

关于返回`本地变量`
在 C++11 之前，返回一个本地对象意味着这个对象会被拷贝，
除非编译器发现可以做返回值优化（named return value optimization，或 NRVO），能把对象直接构造到调用者的栈上。

从 C++11 开始，返回值优化仍可以发生，但在没有返回值优化的情况下，编译器将试图把本地对象移动出去，而不是拷贝出去。
这一行为不需要程序员手工用 std::move 进行干预——使用std::move 对于移动行为没有帮助，反而会影响返回值优化。

总结：用了 std::move 反而妨碍了返回值优化。
*/

#include <iostream>
#include <utility>

using namespace std;

class Obj {
public:
    Obj() { std::cout << "Obj()" << std::endl; }

    Obj(const Obj &) { std::cout << "Obj(const Obj&)" << std::endl; }

    Obj(Obj &&)  noexcept { std::cout << "Obj(Obj&&)" << std::endl; }

    ~Obj() { std::cout << "~Obj()" << std::endl; }
};

Obj simple() {
    Obj obj;
    // 简单返回对象；一般有 NRVO
    return obj;
}

Obj simple_with_move() {
    Obj obj;
    // move 会禁止 NRVO
    return std::move(obj);
}

Obj complicated(int n) {
    Obj obj1;
    Obj obj2;
    // 有分支，一般无 NRVO
    if (n % 2 == 0) {
        return obj1;    // 编译器将试图把本地对象移动出去(调用移动构造函数)
    } else {
        return obj2;    // 编译器将试图把本地对象移动出去(调用移动构造函数)
    }
}


int main() {

    cout << "*** 1 simple ***" << endl;
    {
        auto obj1 = simple();
    }
    cout << "*** 2 simple_with_move ***" << endl;
    {
        auto obj2 = simple_with_move();
    }
    cout << "*** 3 complicated ***" << endl;
    {
        auto obj3 = complicated(42);
    }
    return 0;
}