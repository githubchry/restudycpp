//
// Created by chenr on 2021/4/19.
//

#include <iostream>
#include "PrintProcessor.h"

extern "C" void *Instance() { return new PrintProcessor; }


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
    return 0;
}