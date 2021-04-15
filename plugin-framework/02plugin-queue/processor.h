#pragma once

#include "PluginImpl.h"
class Processor : public PluginImpl
{
private:
    /* data */
public:
    // 获取插件名称
    virtual const char *Name();
    // ==================处理类别插件接口==================
    virtual int ProcessData(ProtocolDataVar *pData);
};


