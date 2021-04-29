#include <iostream>
#include <malloc.h>

// g++ -o main 03fix.cpp -g -fsanitize=address

/*
 * [c++ new 与malloc有什么区别](https://www.cnblogs.com/ywliao/articles/8116622.html)
 *
 * C/C++申请内存有两种方式：
 *  malloc方式: 分配内存(堆)
 *  new方式: 分配内存(自由存储区free store) + 调用构造函数
 * 对应的，释放内存方式：
 *  free： 释放内存
 *  delete：调用析构 + 释放内存
 *
 * 如何调用malloc出来对象的构造函数？
 *  void* mem = malloc(sizeof(S));
 *  S* s = new (mem) S();
 *  s->~S();
 *  free(mem);
 *  这种操作叫做：placement new
 *
 * 但是，仍有漏洞！如果S不是类而是结构体，没有析构函数，而S的成员是类对象，就必须手动将结构体里面的类对象逐个析构！
 *
*/


struct ProtocolDataVar {
    std::string name;
};

int main() {

    void *ptr = malloc(sizeof(ProtocolDataVar));
    //分配了内存，但是name对象没有构造，如果此时操作pData->name，属于未定义行为
    //未定义行为：有可能崩溃，也有可能不会
    ProtocolDataVar *pData = new(ptr)ProtocolDataVar();    // 调用构造函数

    std::cout << ptr << "|" << pData << std::endl;

    std::cout << "1111" << std::endl;
    pData->name = std::string("");
    std::cout << "2222" << std::endl;

    pData->name.~basic_string();   // std::string的析构函数不是~string()

    free(ptr);

    return 0;
}