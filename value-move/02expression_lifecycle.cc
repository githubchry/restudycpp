/*
一个变量（或对象）的生命周期在超出作用域时结束。那临时对象（prvalue）呢？C++的规则是：
    一个临时对象会在包含这个临时对象的完整表达式估值完成后、按生成顺序的逆序被销毁，除非有生命周期延长发生。

C++ 对临时对象有特殊的生命周期延长规则：
    如果一个 prvalue 被绑定到一个引用上，它的生命周期则会延长到跟这个引用变量一样长。

prvalue被绑定到一个应用上，那就是右值引用 => T&&

需要万分注意的是，这条生命期延长规则只对 prvalue 有效，而对 xvalue 无效。
如果由于某种原因，prvalue 在绑定到引用以前已经变成了 xvalue，那生命期就不会延长。
不注意这点的话，代码就可能会产生隐秘的 bug。

你可以把一个没有虚析构函数的子类对象绑定到基类的引用变量上，这个子类对象的析构仍然是完全正常的
 ——这是因为这条规则只是延后了临时对象的析构而已，不是利用引用计数等复杂的方法，因而只要引用绑定成功，其类型并没有什么影响。
*/

#include <iostream>

class Shape {
public:
    Shape() { std::cout << "Shape()" << std::endl; }

    virtual ~Shape() { std::cout << "~Shape()" << std::endl; }
};

class Circle : public Shape {
public:
    Circle() { std::cout << "Circle()" << std::endl; }

    ~Circle() { std::cout << "~Circle()" << std::endl; }
};

class Triangle : public Shape {
public:
    Triangle() { std::cout << "Triangle()" << std::endl; }

    ~Triangle() { std::cout << "~Triangle()" << std::endl; }
};

class Result {
public:
    int val = 10;
    Result() { puts("Result()"); }

    ~Result() { puts("~Result()"); }
};

Result process_shape(const Shape &shape1, const Shape &shape2) {
    puts("process_shape()");
    return Result();
}

int main(){
    std::cout << "一个临时对象会在包含这个临时对象的完整表达式估值完成后、按生成顺序的逆序被销毁，除非有生命周期延长发生。" << std::endl;

    std::cout << "没有生命周期延长情况：" << std::endl;
    // 在这儿，我们生成了临时对象，一个圆和一个三角形，它们会在 process_shape 执行完成并生成结果对象后被销毁。
    process_shape(Circle(), Triangle());
    std::cout << "（Circle 和 Triangle 的顺序在标准中没有规定）" << std::endl;


    std::cout << "右值引用延长生命周期：" << std::endl;
    {
        Result &&result = process_shape(Circle(), Triangle());
        std::cout << "可以看到此时Result还没有被释放" << std::endl;
        std::cout << result.val << std::endl;
    }
    std::cout << "生命周期结束" << std::endl;


    std::cout << "生命期延长规则只对 prvalue 有效，而对 xvalue 无效：" << std::endl;
    {
        Result &&result = std::move(process_shape(Circle(), Triangle()));
        std::cout << "注意看，此时result指向的Result已经被析构了，后面对result解引用是未定义行为。" << std::endl;
        std::cout << "由于 result 指向的是栈空间，项目上通常不会立即导致程序崩溃，而会在某些复杂的组合条件下才会引致问题……" << std::endl;
        std::cout << result.val << std::endl;   // 可能崩溃
    }
    std::cout << "生命周期结束" << std::endl;
}