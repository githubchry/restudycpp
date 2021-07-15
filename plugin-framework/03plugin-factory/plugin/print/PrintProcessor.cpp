//
// Created by chenr on 2021/4/19.
//

#include <iostream>
#include "PrintProcessor.h"

#ifdef DYNAMIC_LOAD_MODE
extern "C" void *Instance() { return new PrintProcessor; }
#endif


const char *PrintProcessor::Name() {
    return "PrintProcessor";
}

bool PrintProcessor::Start() {
    return PluginImpl::Start();
}

bool PrintProcessor::Stop() {
    return PluginImpl::Stop();
}

bool PrintProcessor::Init() {
    return false;
}

bool PrintProcessor::UInit() {
    return false;
}

int PrintProcessor::ProcessData(ProtocolDataVar *pData) {
    static int a = 0;

    a++;
    log_debug("PrintProcessor:%p, %d\n", pData, a);

    if (a >= 5) {
        // 模拟段错误信号
        int *pTmp = NULL;
        *pTmp = 1;	//对未分配内存空间的指针进行赋值，模拟访问非法内存段错误
    }
    return 0;
}