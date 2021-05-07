#include <iostream>
#include <malloc.h>

/*
 * 现象：
 * g++ -o main 01bug.cpp
 * ./main
 * 1111
 * Segmentation fault
 *
*/

struct ProtocolDataVar {
    std::string name;
};

int main() {

    ProtocolDataVar *pData = (ProtocolDataVar *) malloc(sizeof(ProtocolDataVar));
    //分配了内存，但是name对象没有构造，如果此时操作pData->name，属于未定义行为
    //未定义行为：有可能崩溃，也有可能不会

    std::cout << "1111" << std::endl;
    pData->name = "";   // 崩溃
    std::cout << "2222" << std::endl;

    free(pData);

    return 0;
}