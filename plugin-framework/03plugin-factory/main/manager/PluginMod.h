

#pragma once

#include "PluginImpl.h"
#include "PluginLoader.h"
#include <string>
#include <queue>

class PluginMod {
private:

    PluginLoader<GetPluginInterface, PluginImpl *> m_libs;
    std::vector<PluginImpl *> m_vClasses;//端口采集对象集合

    std::queue<ProtocolDataVar *> *pQueue_ = nullptr;//采集数据缓冲区
public:
    PluginMod(/* args */) = default;

    virtual ~PluginMod() = default;

    /**
     * 初始化模块
     * pModuleName: 模块名称(目录路径)，要求所属插件都放到这个目录
     * pInterfaceName: 各插件导出的获取实例接口名称，要求所属插件都统一导出相同名称
    */
    bool Init(const char *pModuleName, const char *pInterfaceName);

    // 释放各模块
    bool UInit();


    // 设置基本信息到模块下的各插件
    void SetDataQueue(std::queue<ProtocolDataVar *> *pQueue);   //队列
    // void SetLogger(ILogger * pLogger);       // 日志
    // void SetMemPool(IMemoryPool *pMemPool);  // 内存池


    // 启动模块
    bool Start();

    // 停止模块
    bool Stop();

    // 释放数据
    int ReleaseData(ProtocolDataVar *pData);

    // 处理数据
    int ProcessData(ProtocolDataVar *pData);
};

