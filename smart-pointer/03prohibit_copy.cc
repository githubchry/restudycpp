/*
operator.cc重载操作符使SmartPtr类对象拥有了指针行为，仍存在以下问题
拷贝该类对象会引发程序行为异常

解决方案有两种
1.禁止拷贝 - 直接在编译阶段提示错误
2.拷贝时转移指针的所有权

下面实现禁止拷贝方案，仅需要增加两行：

//禁止拷贝构造：SmartPtr<Shape> ptr2(ptr1);SmartPtr<Shape> ptr3 = ptr1;
SmartPtr(const SmartPtr&) = delete;
//禁止拷贝赋值：ptr2 = ptr1
SmartPtr& operator=(const SmartPtr&) = delete;
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

  //禁止拷贝构造：SmartPtr<Shape> ptr2(ptr1);
  SmartPtr(const SmartPtr&) = delete;
  //禁止赋值
  SmartPtr& operator=(const SmartPtr&) = delete;

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
  SmartPtr<Shape> ptr1(new Shape());
  // SmartPtr<Shape> ptr2(ptr1);  // 拷贝构造 编译错误
  // SmartPtr<Shape> ptr3{ptr1};  // 拷贝构造 编译错误
  // SmartPtr<Shape> ptr4 = ptr1; // 拷贝构造 编译错误
  // ptr4 = ptr1; // 拷贝赋值 编译错误

  std::cout << "利用delete关键字实现类对象禁止拷贝功能" << std::endl
            << "注意不要跟explicit语义弄混淆" << std::endl;

  // 有的时候，这种语法行为不一定应该禁止，可以考虑另一种方案：
  // 拷贝时转移指针的所有权
  return 0;
}