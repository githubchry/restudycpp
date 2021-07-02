/*
移动的意义 —— 减少运行的开销

示例：
    string result = string("Hello, ") + name + ".";

在 C++11 之前的年代里，这种写法是绝对不推荐的。因为它会引入很多额外开销，执行流程大致如下：
1. 调用构造函数 string(const char*)，生成临时对象 1；"Hello, " 复制 1 次。
2. 调用 operator+(const string&, const string&)，生成临时对象 2；"Hello," 复制 2 次，name 复制 1 次。
3. 调用 operator+(const string&, const char*)，生成对象 3；"Hello, " 复制 3 次，name 复制 2 次，"." 复制 1 次。
4. 假设返回值优化能够生效（最佳情况），对象 3 可以直接在 result 里构造完成。
5. 临时对象 2 析构，释放指向 string("Hello, ") + name 的内存。
6. 临时对象 1 析构，释放指向 string("Hello, ") 的内存。

从 C++11 开始，这不再是必须的。同样上面那个单行的语句，执行流程大致如下：
1. 调用构造函数 string(const char*)，生成临时对象 1；"Hello, " 复制 1 次。
2. 调用 operator+(string&&, const string&)，直接在临时对象 1 上面执行追加操作，并把结果移动到临时对象 2；name 复制 1 次。
3. 调用 operator+(string&&, const char*)，直接在临时对象 2 上面执行追加操作，并把结果移动到 result；"." 复制 1 次。
4. 临时对象 2 析构，内容已经为空，不需要释放任何内存。
5. 临时对象 1 析构，内容已经为空，不需要释放任何内存。


此外很关键的一点是，C++ 里的对象缺省都是值语义。在下面这样的代码里：
    class A {
        B b_;
        C c_;
    };
从实际内存布局的角度，很多语言——如 Java 和 Python——会在 A 对象里放 B 和 C 的指针（虽然这些语言里本身没有指针的概念）。
而 C++ 则会直接把 B 和 C 对象放在 A 的内存空间里。这种行为既是优点也是缺点。
说它是优点，是因为它保证了内存访问的局域性，而局域性在现代处理器架构上是绝对具有性能优势的。
说它是缺点，是因为复制对象的开销大大增加：在 Java 类语言里复制的是指针，在 C++ 里是完整的对象。
这就是为什么C++ 需要移动语义这一优化，而 Java 类语言里则根本不需要这个概念。

一句话总结，移动语义使得在 C++ 里返回大对象（如容器）的函数和运算符成为现实，因而可以提高代码的简洁性和可读性，提高程序员的生产率。

所有的现代 C++ 的标准容器都针对移动进行了优化。

如何实现移动？——要让你设计的对象支持移动的话，通常需要下面几步：
1. 你的对象应该有分开的拷贝构造和移动构造函数（除非你只打算支持移动，不支持拷贝——如 unique_ptr）。
2. 你的对象应该有 swap 成员函数，支持和另外一个对象快速交换成员。
3. 在你的对象的命名空间下，应当有一个全局的 swap 函数，调用成员函数 swap 来实现交换。支持这种用法会方便别人（包括你自己在将来）在其他对象里包含你的对象，并快速实现它们的 swap 函数。
4. 实现通用的 operator=。

上面各个函数如果不抛异常的话，应当标为 noexcept。这对移动构造函数尤为重要。
具体写法可以参考我们之前已经实现的智能指针： smart-pointer/08pointer_type_conversion.cc
*/

#include <iostream>

class SharedCount {
public:
    SharedCount() : count_(1) {}

    void AddCount() { ++count_; }

    long Reduce() { return --count_; }

    long GetCount() const { return count_; }

private:
    long count_;
};

template<typename T>
class SmartPtr {
    /// 模板的各个实例间并不天然就有 friend 关系，因而不能互访私有成员 ptr_ 和 sharedCount_。所以需要在 SmartPtr 的定义中显式声明:
    template<typename U>
    friend
    class SmartPtr;

private:
    T *ptr_;
    SharedCount *sharedCount_;

public:
    explicit SmartPtr(T *ptr = nullptr) : ptr_(ptr) {
        if (ptr) {
            sharedCount_ = new SharedCount();
        }
    }

    SmartPtr(const SmartPtr &other) noexcept {
        ptr_ = other.ptr_;
        if (ptr_) {
            other.sharedCount_->AddCount();
            sharedCount_ = other.sharedCount_;
        }
    }

    ~SmartPtr() {
        if (ptr_ && !sharedCount_->Reduce()) {
            delete ptr_;
            delete sharedCount_;
        }
    }

    // todo: 1. 你的对象应该有分开的拷贝构造和移动构造函数（除非你只打算支持移动，不支持拷贝——如 unique_ptr）。
    /**
     * 拷贝构造函数
     * 需要在指针非空时把引用数加一，并复制共享计数的指针。
     */
    template<typename U>
    SmartPtr(const SmartPtr<U> &other) noexcept {
        ptr_ = other.ptr_;
        if (ptr_) {
            other.sharedCount_->AddCount();
            sharedCount_ = other.sharedCount_;
        }
    }

    /**
     * 移动构造函数
     * 从另一个对象获取资源，清空其资源，并将其置为一个可析构的状态。
     * 不需要调整引用数，直接把 other.ptr_ 置为空，认为 other 不再指向该共享对象即可。
     */
    template<typename U>
    SmartPtr(SmartPtr<U> &&other) noexcept {
        ptr_ = other.ptr_;
        if (ptr_) {
            sharedCount_ = other.sharedCount_;
            other.ptr_ = nullptr;
        }
    }

    // todo: 2. 你的对象应该有 swap 成员函数，支持和另外一个对象快速交换成员。
    void swap(SmartPtr &rhs) noexcept {
        std::swap(sharedCount_, rhs.sharedCount_);
        std::swap(ptr_, rhs.ptr_);
    }

    /**
     * todo: 4. 实现通用的 operator=。
     *
     * 拷贝赋值函数：在构造参数时直接生成新的智能指针，从而不再需要在函数体中构造临时对象。
     * 赋值函数的行为是移动还是拷贝，完全依赖于构造参数时走的是移动构造还是拷贝构造。
     *
     * 注意为了避免让人吃惊，通常我们需要将其实现成对 a = a; 这样的写法安全。
     * 下面的写法算是个小技巧，对传递左值和右值都有效，而且规避了 if (&rhs != this) 这样的判断。
     */
    SmartPtr &operator=(SmartPtr rhs) {
        rhs.swap(*this);
        return *this;
    };
};

// todo: 3. 在你的对象的命名空间下，应当有一个全局的 swap 函数，调用成员函数 swap 来实现交换。
template<typename T>
void swap(SmartPtr<T> &lhs, SmartPtr<T> &rhs) noexcept {
    lhs.swap(rhs);
}

// =====================================================================================================================


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

    SmartPtr<Circle> ptr1(new Circle());
    SmartPtr<Shape> ptr2;

    ptr2 = ptr1;  //共享对象

    ptr2 = std::move(ptr1);  // 转移所有权 引用减一


    std::cout << "没有生命周期延长情况：" << std::endl;
    // 在这儿，我们生成了临时对象，一个圆和一个三角形，它们会在 process_shape 执行完成并生成结果对象后被销毁。
    SmartPtr<Circle>(new Circle());
    std::cout << "（Circle 和 Triangle 的顺序在标准中没有规定）" << std::endl;


    std::cout << "右值引用延长生命周期：" << std::endl;
    {
        SmartPtr<Circle> &&circle = SmartPtr<Circle>(new Circle());
        std::cout << "可以看到此时 circle 还没有被释放" << std::endl;
    }
    std::cout << "生命周期结束" << std::endl;


    return 0;
}