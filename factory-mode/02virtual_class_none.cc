#include <string.h>

#include <iostream>

enum class shape_type {
  circle,    //圆形
  triangle,  //三角形
  cone,      //圆锥形
};

/*
       菱形继承
        Shape
      ↙     ↘
  Circle    Triangle
      ↘     ↙
        Cone

创建一个Cone会生成两份Shape
*/

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
};

class Cone : public Triangle, public Circle {};

int main() {
  // 返回的是实际上是Shape的子类
  Cone* shape = new Cone();

  // strncpy(shape->shapeBuf, "hello world", 256);  // 歧义，编译不通过
  strncpy(shape->Circle::shapeBuf, "hello Circle", 256);
  strncpy(shape->Triangle::shapeBuf, "hello Triangle", 256);

  // std::cout << shape->shapeBuf << std::endl; // 歧义，编译不通过
  std::cout << shape->Circle::shapeBuf << std::endl;
  std::cout << shape->Triangle::shapeBuf << std::endl;

  delete shape;

  std::cout << "可以看到打印了两次new Shape buf" << std::endl;

  return 0;
}