
/*
unique_ptr 算是一种较为安全的智能指针了。
但是，一个对象只能被单个 unique_ptr所拥有，这显然不能满足所有使用场合的需求。
一种常见的情况是，多个智能指针同时拥有一个对象；当它们全部都失效时，这个对象也同时会被删除。
这也就是 shared_ptr 了。

unique_ptr 和 shared_ptr 的主要区别
┌────────────┐      ┌────────────┐              ┌────────────┐
│ unique_ptr │      │ shared_ptr ├──┐        ┌──┤ shared_ptr │
└─────┬──────┘      └─────┬──────┘  │        │  └─────┬──────┘
      │                   │    ┌────↓────────↓────┐   │
      │                   │    │  reference count │   │
      │                   │    └──────────────────┘   │
┌─────↓──────┐            │       ┌────────────┐      │
│   object   │            └──────→│   object   │←─────┘
└────────────┘                    └────────────┘

多个不同的 shared_ptr 不仅可以共享一个对象，在共享同一对象时也需要同时共享同一
个计数。当最后一个指向对象（和共享计数）的 shared_ptr 析构时，它需要删除对象和
共享计数。我们下面就来实现一下。
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
  SmartPtr(SmartPtr<U>& other) {
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
  SmartPtr(SmartPtr<U>&& other) {
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

 private:
  T* ptr_;
  SharedCount* sharedCount_;

  void swap(SmartPtr& rhs) {
    std::swap(sharedCount_, rhs.sharedCount_);
    std::swap(ptr_, rhs.ptr_);
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
  SmartPtr<Circle> ptr1(new Circle());
  SmartPtr<Shape> ptr2;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;


  ptr2 = ptr1;  //共享对象
  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;

  ptr2 = std::move(ptr1); // 转移所有权
  std::cout << std::endl;
  std::cout << "ptr1:" << ptr1.UseCount() << std::endl;
  std::cout << "ptr2:" << ptr2.UseCount() << std::endl;


  std::cout << "一个比较完整的引用计数智能指针的实现" << std::endl;

  return 0;
}