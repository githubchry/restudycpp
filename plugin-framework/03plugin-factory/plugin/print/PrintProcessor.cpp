//
// Created by chenr on 2021/4/19.
//

#include <iostream>
#include <cassert>
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
    log_debug("PrintProcessor:%p\n", pData);

    static int a = 0;
    if (++a >= 5) {
        // 模拟段错误信号 => SIGSEGV
        //int *pTmp = NULL;
        //*pTmp = 1;	//对未分配内存空间的指针进行赋值，模拟访问非法内存段错误

        // double-free => SIGABRT
        void *pc = malloc(1024);
        free(pc);
        free(pc);

        // 执行abort()函数 => SIGABRT
        // abort();

        // 断言 => SIGABRT
        //void *tmp = NULL;
        //assert( tmp != NULL );
    }
    return 0;
}