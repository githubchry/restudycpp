/*
operator.cc重载操作符使SmartPtr类对象拥有了指针行为，仍存在以下问题
拷贝该类对象会引发程序行为异常

解决方案有两种
1.禁止拷贝 - 直接在编译阶段提示错误
2.拷贝时转移指针的所有权

下面实现方案二

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

  //拷贝构造函数：转移指针的所有权 SmartPtr<Shape> ptr(ptr1);
  SmartPtr(SmartPtr& other) { ptr_ = other.release(); }

  //拷贝赋值函数：转移指针的所有权 ptr1 = ptr2;
  SmartPtr& operator=(SmartPtr& rhs) {
    //通过拷贝构造产生一个临时对象并调用 swap 来交换对指针的所有权
    SmartPtr(rhs).swap(*this);
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

int main() {
  SmartPtr<Shape> ptr1(new Shape());  // 构造
  SmartPtr<Shape> ptr2(ptr1);         // 拷贝构造
  SmartPtr<Shape> ptr3{ptr1};         // 拷贝构造
  SmartPtr<Shape> ptr4 = ptr1;        // 拷贝构造

  std::cout << "ptr1:" << ptr1 << std::endl;
  std::cout << "ptr2:" << ptr2 << std::endl;
  std::cout << "ptr3:" << ptr3 << std::endl;
  std::cout << "ptr4:" << ptr4 << std::endl;

  ptr1 = ptr2;  // 拷贝赋值
  std::cout << "ptr1 = ptr2" << std::endl;

  std::cout << "ptr1:" << ptr1 << std::endl;
  std::cout << "ptr2:" << ptr2 << std::endl;

  std::cout << "重载拷贝构造函数和拷贝赋值函数，实现指针所有权转移功能"
            << std::endl;

  /*上面实现的最大问题是，它的行为会让程序员非常容易犯错。一不小心把它传递给另外一个SmartPtr，你就不再拥有这个对象了……*/
  return 0;
}