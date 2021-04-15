#pragma once

#include "PluginImpl.h"
#include <thread>
#include <chrono>

class Collector : public PluginImpl
{
private:
    /* data */
    std::queue<ProtocolDataVar *> *pQueue_ = nullptr;
    bool isRuning = false;
    std::thread thread_;

public:
    // 获取插件名称
    virtual const char *Name();
    virtual bool Start();
    virtual bool Stop();

    // ==================生产类别插件接口==================
    virtual int ReleaseData(ProtocolDataVar *pData);
    virtual void SetDataQueue(std::queue<ProtocolDataVar *> *pQueue);
};
