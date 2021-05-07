#pragma once

#include <memory>
#include <mutex>
#include <iostream>

//[C++11新特性之default/delete](https://blog.csdn.net/lixiaogang_theanswer/article/details/81090622)
//permission => pms     用作继承单例模板基类时应为protected，用作自身时应为private
#define SINGLETON_CTOR(cls, initpms, exitpms) \
initpms:                                      \
    cls() { std::cout << this << std::endl; } \
exitpms:                                      \
    ~cls() = default;                         \
private:                                      \
    cls(const cls &) = delete;                \
    cls &operator=(const cls &) = delete;

//在栈建立实例
#define SINGLETON_STACK(cls)              \
public:                                   \
    static cls &Instance()                \
    {                                     \
        static cls t;                     \
        return t;                         \
    }

//在堆new出实例
#define SINGLETON_HEAP(cls)                                   \
public:                                                       \
    static cls &Instance()                                    \
    {                                                         \
        static std::unique_ptr<cls> _ptr;                     \
        static std::once_flag flag;                           \
        std::call_once(flag, [&]() { _ptr.reset(new cls); }); \
        return *_ptr;                                         \
    }

#define SINGLETON_STACK_CTOR(cls)         \
    SINGLETON_CTOR(cls, private, private) \
    SINGLETON_STACK(cls)

#define SINGLETON_HEAP_CTOR(cls)          \
    SINGLETON_CTOR(cls, private, public) \
    SINGLETON_HEAP(cls)

/*                                                                                                                                                          \
类的特殊成员函数：默认构造函数，析构函数，复制构造函数，拷贝复制运算符，取址运算符，const取址运算符          \
如果程序员在类里面没有定义特殊成员函数做出定义，在使用过程中编译器会自动隐式生成                                    \
default：C++11新特性，自动生成函数体，效率比手动增加高，仅适用于类的特殊成员函数;                                         \
    比如类含有构造函数时，不会为其生成默认构造函数，使用default可以自动生成一个默认的的构造函数（没有参数） \
delete：C++11新特性，禁用函数，防止该函数被调用，可作用于特殊成员函数/成员函数/普通函数                                \
    比如单例模式里，通常不允许这个类被复制和赋值，有的还不允许手动释放，就可以用delete声明相关函数              \
*/

template<typename T>
class SingletonStack {
    SINGLETON_CTOR(SingletonStack, protected, protected);
    SINGLETON_STACK(T);
};

template<typename T>
class SingletonHeap {
    SINGLETON_CTOR(SingletonHeap, protected, protected); //析构函数
    SINGLETON_HEAP(T);
};
