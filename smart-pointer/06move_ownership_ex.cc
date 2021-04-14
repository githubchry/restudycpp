
/*
05move_ownership.cc存在的问题：
子类指针向基类指针的转换
SmartPtr<Circle> 无法自动转为 SmartPtr<Shape>
这个行为显然还是不够“自然”

只需要额外加一点模板代码，就能实现这一行为。在我们目前给出的实现里，只需要
修改我们的移动构造函数一处即可————这也算是我们让赋值函数使用拷贝/移动构造函数的好处了。

template <typename U>
SmartPtr(SmartPtr<U>&& other){ptr_ = other.release();}

这样，我们自然而然利用了指针的转换特性：

现在 SmartPtr<circle> 可以移动给SmartPtr<shape>，但不能移动给
SmartPtr<triangle>。 不正确的转换会在代码编译时直接报错。
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
  template <typename U>
  SmartPtr(SmartPtr<U>&& other) {
    ptr_ = other.release();
  }

  // 拷贝赋值函数：在构造参数时直接生成新的智能指针，从而不再需要在函数体中构造临时对象
  // 赋值函数的行为是移动还是拷贝，完全依赖于构造参数时走的是移动构造还是拷贝构造。
  SmartPtr& operator=(SmartPtr rhs) {
    rhs.swap(*this);
    return *this;
  };

  // 释放对指针的所有权
  T* release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

 private:
  T* ptr_;

  void swap(SmartPtr& rhs) { std::swap(ptr_, rhs.ptr_); }
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
  SmartPtr<Circle> ptr1(new Circle());

  SmartPtr<Shape> ptr2;

  ptr2 = std::move(ptr1);

  SmartPtr<Shape> ptr3{std::move(ptr2)};

  std::cout << "ptr1:" << ptr1 << std::endl;
  std::cout << "ptr2:" << ptr2 << std::endl;
  std::cout << "ptr3:" << ptr3 << std::endl;

  std::cout << "实现了子类指针向基类指针的自动转换行为" << std::endl;
  std::cout << "实现了 C++11 的 unique_ptr 的基本行为" << std::endl;

  return 0;
}