#include <iostream>
#include <map>

enum class shape_type {
    circle,    //圆形
    triangle,  //三角形
    rectangle  //矩形
};

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

    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    char *circleBuf = nullptr;

    Circle() {
        circleBuf = new char[256];
        std::cout << "new Circle buf" << std::endl;
    }

    ~Circle() {
        std::cout << "delete Circle buf" << std::endl;
        delete[] circleBuf;
    }

    void draw() override {
        std::cout << "draw Circle" << std::endl;
    }
};

class Triangle : public Shape {
public:
    char *triangleBuf = nullptr;

    Triangle() {
        triangleBuf = new char[256];
        std::cout << "new Triangle buf" << std::endl;
    }

    ~Triangle() {
        std::cout << "delete Triangle buf" << std::endl;
        delete[] triangleBuf;
    }

    void draw() override {
        std::cout << "draw Triangle" << std::endl;
    }
};

class ShapeWrapper {
public:
    explicit ShapeWrapper(Shape *ptr = nullptr) : ptr_(ptr) {}

    ~ShapeWrapper() { delete ptr_; }

    Shape *get() const { return ptr_; }

private:
    Shape *ptr_;
};

Shape *create_shape(shape_type type) {
    switch (type) {
        case shape_type::circle:
            return new Circle();
        case shape_type::triangle:
            return new Triangle();
        default:
            return nullptr;
    }
}

int main() {

    // ShapeWrapper shapeWrapper = create_shape(shape_type::circle); //使用explicit禁用了该形式的初始化
    ShapeWrapper shapeWrapper(create_shape(shape_type::circle));

    std::cout
            << "利用RAII特性，不需要手动delete，降低内存泄漏的风险，也可以实现"
               "锁的自动解除功能"
            << std::endl
            << "需要注意使用virtual避开对象切片(object slicing)导致的内存泄漏陷阱"
            << std::endl;


    // bug 示范：
    std::map<std::string, ShapeWrapper> shapeMap;  // <name, thread>
    {
        // ShapeWrapper shapeWrapper = create_shape(shape_type::circle); //使用explicit禁用了该形式的初始化
        ShapeWrapper shapeWrapper(create_shape(shape_type::circle));
        shapeMap["aaa"] = shapeWrapper;
    }
    shapeMap["aaa"].get()->draw();

    std::cout << "over" << std::endl;
    // 进程已结束，退出代码为 1, 正常应该返回0
    return 0;
}