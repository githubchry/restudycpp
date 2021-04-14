
/*
对应于 C++ 里的不同的类型强制转换：
static_cast
reinterpret_cast
const_cast
dynamic_cast
智能指针需要实现类似的函数模板。实现本身并不复杂，但为了实现这些转换，我们需要添
加构造函数，允许在对智能指针内部的指针对象赋值时，使用一个现有的智能指针的共享计
数。
*/

#include <string.h>

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

template <typename T>
class SmartPtr {
  /**
   * 模板的各个实例间并不天然就有 friend 关系，
   * 因而不能互访私有成员 ptr_ 和 sharedCount_。
   * 所以需要在 SmartPtr 的定义中显式声明:
   */
  template <typename U>
  friend class SmartPtr;

 public:
  explicit SmartPtr(T* ptr = nullptr) : ptr_(ptr) {
    if (ptr) {
      sharedCount_ = new SharedCount();
    }
  }

  // 实现不同的类型强制转换
  template <typename U>
  SmartPtr(const SmartPtr<U>& other, T* ptr = nullptr) : ptr_(ptr) {
    if (ptr) {
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

  T& operator*() const { return *ptr_; }  //用 * 运算符解引用
  T* operator->() const { return ptr_; }  //用 -> 运算符指向对象成员
  operator bool() const { return ptr_; }  //像指针一样用在布尔表达式里

  /**
   * 拷贝构造函数
   * 需要在指针非空时把引用数加一，并复制共享计数的指针。
   */
  template <typename U>
  SmartPtr(SmartPtr<U>& other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
      other.sharedCount_->AddCount();
      sharedCount_ = other.sharedCount_;
    }
  }

  /**
   * 移动构造函数
   * 不需要调整引用数，直接把 other.ptr_ 置为空，认为 other
   * 不再指向该共享对象即可。
   */
  template <typename U>
  SmartPtr(SmartPtr<U>&& other) noexcept {
    ptr_ = other.ptr_;
    if (ptr_) {
      sharedCount_ = other.sharedCount_;
      other.ptr_ = nullptr;
    }
  }

  // 拷贝赋值函数：在构造参数时直接生成新的智能指针，从而不再需要在函数体中构造临时对象
  // 赋值函数的行为是移动还是拷贝，完全依赖于构造参数时走的是移动构造还是拷贝构造。
  SmartPtr& operator=(SmartPtr rhs) {
    rhs.swap(*this);
    return *this;
  };

  // 加一个对调试非常有用的函数，返回引用计数值
  long UseCount() const {
    if (ptr_) {
      return sharedCount_->GetCount();
    } else {
      return 0;
    }
  }

  // noexcept对这个智能指针在它的目标场景能正确使用是十分必要的
  T* Get() const noexcept { return ptr_; };

  void swap(SmartPtr& rhs) noexcept {
    std::swap(sharedCount_, rhs.sharedCount_);
    std::swap(ptr_, rhs.ptr_);
  }

 private:
  T* ptr_;

  SharedCount* sharedCount_;
};
//-----------------------------------------------------------------------------

template <typename T>
void swap(SmartPtr<T>& lhs, SmartPtr<T>& rhs) noexcept {
  lhs.swap(rhs);
}

/*
static_cast
reinterpret_cast
const_cast
dynamic_cast
*/
template <typename T, typename U>
SmartPtr<T> static_pointer_cast(const SmartPtr<U>& other) {
  T* ptr = static_cast<T*>(other.Get());
  return SmartPtr<T>(other, ptr);
}

template <typename T, typename U>
SmartPtr<T> reinterpret_pointer_cast(const SmartPtr<U>& other) {
  T* ptr = reinterpret_cast<T*>(other.Get());
  return SmartPtr<T>(other, ptr);
}

template <typename T, typename U>
SmartPtr<T> const_pointer_cast(const SmartPtr<U>& other) {
  T* ptr = const_cast<T*>(other.Get());
  return SmartPtr<T>(other, ptr);
}

template <typename T, typename U>
SmartPtr<T> dynamic_pointer_cast(const SmartPtr<U>& other) {
  T* ptr = dynamic_cast<T*>(other.Get());
  return SmartPtr<T>(other, ptr);
}
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
  SmartPtr<Shape> ptr0(create_shape(shape_type::circle));
  SmartPtr<Circle> ptr1(new Circle());
  SmartPtr<Shape> ptr2;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;

  ptr2 = ptr1;  //共享对象
  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;

  // 编译错误，因为const_cast不允许将'Shape*'转为'Circle*'
  // SmartPtr<Circle> ptr6 = const_pointer_cast<Circle>(ptr2);
  SmartPtr<Circle> ptr6 = const_pointer_cast<Circle>(ptr1);
  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr6:" << ptr6.UseCount() << std::endl;

  ptr2 = std::move(ptr1);  // 转移所有权 引用减一
  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;
  std::cout << "ptr6:" << ptr6.UseCount() << std::endl;

  SmartPtr<Circle> ptr3 = static_pointer_cast<Circle>(ptr2);
  SmartPtr<Circle> ptr4 = reinterpret_pointer_cast<Circle>(ptr2);
  SmartPtr<Circle> ptr5 = dynamic_pointer_cast<Circle>(ptr2);

  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;
  std::cout << "ptr3:" << ptr3.UseCount() << std::endl;
  std::cout << "ptr4:" << ptr4.UseCount() << std::endl;
  std::cout << "ptr5:" << ptr5.UseCount() << std::endl;

  swap(ptr1, ptr5);
  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr5:" << ptr5.UseCount() << std::endl;

  std::cout << "一个比较完整的引用计数智能指针的实现" << std::endl;

  return 0;
}