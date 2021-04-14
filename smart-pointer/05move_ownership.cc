/*
transfer_ownership.cc实现了拷贝时转移指针的所有权行为
里面面实现的最大问题是，它的行为会让程序员非常容易犯错。
一不小心把它传递给另外一个 SmartPtr，你就不再拥有这个对象了……

这里使用“移动”来改善其行为
*/

#include <string.h>

#include <iostream>

template <typename T>
class SmartPtr {
 public:
  explicit SmartPtr(T* ptr = nullptr) : ptr_(ptr) {}
  ~SmartPtr() { delete ptr_; }

  T& operator*() const { return *ptr_; }  //用 * 运算符解引用
  T* operator->() const { return ptr_; }  //用 -> 运算符指向对象成员
  operator bool() const { return ptr_; }  //像指针一样用在布尔表达式里

  // 移动构造函数 => 将自动禁用拷贝构造函数，除非手动声明了拷贝构造函数
  SmartPtr(SmartPtr&& other) { ptr_ = other.release(); }

  // 拷贝赋值函数：在构造参数时直接生成新的智能指针，从而不再需要在函数体中构造临时对象
  // 赋值函数的行为是移动还是拷贝，完全依赖于构造参数时走的是移动构造还是拷贝构造。
  SmartPtr& operator=(SmartPtr rhs) {
    rhs.swap(*this);
    return *this;
  };

 private:
  T* ptr_;

  void swap(SmartPtr& rhs) { std::swap(ptr_, rhs.ptr_); }

  // 释放对指针的所有权
  T* release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }
};

//=============================================================================

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
    delete shapeBuf;
  }
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
    delete circleBuf;
  }
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
    delete triangleBuf;
  }
};

class ShapeWrapper {
 public:
  explicit ShapeWrapper(Shape* ptr = nullptr) : ptr_(ptr) {}
  ~ShapeWrapper() { delete ptr_; }
  Shape* get() const { return ptr_; }

 private:
  Shape* ptr_;
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
  SmartPtr<Shape> ptr1(new Shape());  // 构造

  SmartPtr<Shape> ptr2;

  // ptr2 = ptr1; // 拷贝赋值函数的行为是移动不是拷贝 编译出错
  ptr2 = std::move(ptr1);
  SmartPtr<Shape> ptr3{std::move(ptr2)};

  // SmartPtr<Shape> ptr4(ptr1);         // 拷贝构造被自动禁用 编译出错
  // SmartPtr<Shape> ptr5{ptr1};         // 拷贝构造被自动禁用 编译出错
  // SmartPtr<Shape> ptr6 = ptr1;        // 拷贝构造被自动禁用 编译出错

  std::cout << "ptr1:" << ptr1 << std::endl;
  std::cout << "ptr2:" << ptr2 << std::endl;
  std::cout << "ptr3:" << ptr3 << std::endl;

  std::cout << "实现了 C++11 的 unique_ptr 的基本行为" << std::endl;

  /**
   * 存在的问题：子类指针向基类指针的转换
   * SmartPtr<Circle> 无法自动转为 SmartPtr<Shape>
   * 这个行为显然还是不够“自然”
   */
  // SmartPtr<Circle> ptr7(new Circle());
  // SmartPtr<Shape> ptr8{std::move(ptr7)};  // 编译错误

  return 0;
}