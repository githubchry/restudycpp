#include <iostream>
#include <malloc.h>

/*
 * 现象：
 * g++ -o main 02bug.cpp
 * ./main
 * 1111
 * 2222
 *
 * 运行正常，但是如果改一下编译参数：
 *
 * g++ -o main 02bug.cpp -g -fsanitize=address
 * ./main
 * 1111
 * AddressSanitizer:DEADLYSIGNAL
 * ......崩溃打印......
*/

struct ProtocolDataVar {
    std::string name;
};

int main() {

    ProtocolDataVar *pData = (ProtocolDataVar *) malloc(sizeof(ProtocolDataVar));

    //分配了内存，但是name对象没有构造，如果此时操作pData->name，属于未定义行为
    //未定义行为：有可能崩溃，也有可能不会

    std::cout << "1111" << std::endl;
    pData->name = "str";    // 如果赋值为空串""必定崩溃；如果是非空串不一定崩溃，调整编译参数可以复现崩溃现象
    std::cout << "2222" << std::endl;

    free(pData);

    return 0;
}