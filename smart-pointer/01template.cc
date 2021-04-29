/*
raii_wrapper.cc实现了成智能指针的最基本的功能：对超出作用域的对象进行释放。但它缺了点东
西：
1. 这个类只适用于 shape 类
2. 该类对象的行为不够像指针
3. 拷贝该类对象会引发程序行为异常


下面解决第1个问题。
模板化和易用性
要让这个类能够包装任意类型的指针，我们需要把它变成一个类模板。这实际上相当容易：
*/

#include <string.h>

#include <iostream>

template <typename T>
class SmartPtr {
 public:
  explicit SmartPtr(T* ptr = nullptr) : ptr_(ptr) {}
  ~SmartPtr() { delete ptr_; }
  T* get() const { return ptr_; }

 private:
  T* ptr_;
};

class Shape {
 public:
  char* shapeBuf = nullptr;
  Shape() {
    shapeBuf = new char[256];
    std::cout << "new Shape buf" << std::endl;
  }
  ~Shape() {
    std::cout << "delete Shape buf" << std::endl;
    delete[] shapeBuf;
  }
};

int main() {
  SmartPtr<Shape> smartPtr(new Shape());

  strcpy(smartPtr.get()->shapeBuf, "hello world!");

  *smartPtr.get()->shapeBuf = 'H';

  if (smartPtr.get()) {
    std::cout << smartPtr.get()->shapeBuf << std::endl;
  }

  std::cout << "利用template特性，使SmartPtr类能包装任意类型的指针"
            << std::endl;

  // 准备解决的问题：
  // 该类对象的行为不够像指针：
  // 不能用 * 运算符解引用
  // 不能用 -> 运算符指向对象成员
  // 不能像指针一样用在布尔表达式里

  /*
  strcpy(smartPtr->shapeBuf, "hello world!");

  *smartPtr->shapeBuf = 'H';

  if (smartPtr)
  {
    std::cout << smartPtr->shapeBuf << std::endl;
  }
  */

  return 0;
}