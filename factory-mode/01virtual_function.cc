#include <iostream>

enum class shape_type {
  circle,    //圆形
  triangle,  //三角形
  rectangle  //矩形
};

class Shape {
 public:
  char* shapeBuf = nullptr;
  Shape() {
    shapeBuf = new char[256];
    std::cout << "new Shape buf" << std::endl;
  }
  virtual ~Shape() {
    std::cout << "delete Shape buf" << std::endl;
    delete[] shapeBuf;
  }
  // 虚函数：该方法子类可以重写
  virtual void draw() { std::cout << "draw shape" << std::endl; }
  // 纯虚函数：要求派生类必须实现，否则会有编译错误
  virtual void type() = 0;

  void name() { std::cout << "name shape" << std::endl; }
};

class Circle : public Shape {
 public:
  char* circleBuf = nullptr;
  Circle() {
    circleBuf = new char[256];
    std::cout << "new Circle buf" << std::endl;
  }
  ~Circle() {
    std::cout << "delete Circle buf" << std::endl;
    delete[] circleBuf;
  }
  void draw() { std::cout << "draw circle" << std::endl; }
  void name() { std::cout << "name circle" << std::endl; }
  void type() { std::cout << "type circle" << std::endl; }
};

class Triangle : public Shape {
 public:
  char* triangleBuf = nullptr;
  Triangle() {
    triangleBuf = new char[256];
    std::cout << "new Triangle buf" << std::endl;
  }
  ~Triangle() {
    std::cout << "delete Triangle buf" << std::endl;
    delete[] triangleBuf;
  }
  void draw() { std::cout << "draw triangle" << std::endl; }
  void name() { std::cout << "name triangle" << std::endl; }
  void type() { std::cout << "type triangle" << std::endl; }
};

Shape* create_shape(shape_type type) {
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
  // 返回的是实际上是Shape的子类
  Shape* shape = create_shape(shape_type::circle);

  //重写(override),也叫覆盖。子类重新定义父类中有相同名称和参数的虚函数(virtual)。在继承关系之间。C++利用虚函数实现多态。
  //在基类中将被重写的成员函数设置为虚函数，其含义是：
  //当通过基类的指针或者引用调用该成员函数时，将根据指针指向的对象类型确定调用的函数，而非指针的类型。

  shape->draw();  // 虚函数打印：circle
  shape->type();  // 纯虚函数打印：circle
  shape->name();  // 非虚函数打印：shape  => 对象切片(object slicing)陷阱
  ((Circle*)shape)->name();  // 强转后非虚函数打印：circle

  delete shape;

  std::cout << "基类析构函数声明为虚函数，解决内存泄漏问题" << std::endl;

  return 0;
}