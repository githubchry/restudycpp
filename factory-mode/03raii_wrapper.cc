#include <iostream>

class Shape {
 public:
  char* shapeBuf = nullptr;
  Shape() {
    shapeBuf = new char[256];
    std::cout << "new Shape buf" << std::endl;
  }
  ~Shape() {
    std::cout << "delete Shape buf" << std::endl;
    delete shapeBuf;
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

int main() {
  // ShapeWrapper shapeWrapper = new Shape(); //使用explicit禁用了该形式的初始化
  ShapeWrapper shapeWrapper(new Shape());

  std::cout << "利用RAII特性，不需要手动delete，降低内存泄漏的风险，也可以实现"
               "锁的自动解除功能"
            << std::endl;

  return 0;
}