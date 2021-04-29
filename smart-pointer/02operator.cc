/*
template.cc使用模板使智能指针SmartPtr能包装任意类型的指针，仍存在以下问题
1. 该类对象的行为不够像指针
2. 拷贝该类对象会引发程序行为异常

下面分析解决类对象的行为不够像指针问题：
它不能用 * 运算符解引用
它不能用 -> 运算符指向对象成员
它不能像指针一样用在布尔表达式里
这些问题也相当容易解决，加几个成员函数就可以取代get()函数：
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

  strcpy(smartPtr->shapeBuf, "hello world!");

  *smartPtr->shapeBuf = 'H';

  if (smartPtr) {
    std::cout << smartPtr->shapeBuf << std::endl;
  }

  std::cout << "利用operator特性，重载的操作符使SmartPtr类能实现指针行为"
            << std::endl;

  // 准备解决的问题，以下语句会出错，导致对同一内存释放两次，通常情况下会导致程序崩溃：
  // SmartPtr<Shape> ptr2{smartPtr};
  // SmartPtr<Shape> ptr3 = smartPtr;
  // 别以为析构里面执行delete shapeBuf;后再shapeBuf = nullptr;就能解决，傻逼。
  // 问题的关键在于：我们该如何定义其行为
  // 将分别实现两种行为方案：
  // 1.禁止拷贝 - 直接在编译阶段提示错误
  // 2.拷贝时转移指针的所有权

  return 0;
}