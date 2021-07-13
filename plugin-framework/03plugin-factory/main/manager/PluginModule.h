

#pragma once


#ifdef INLINE
#include "PluginInliner.h"
#else
#include "PluginLoader.h"
#endif
#include "PluginImpl.h"

class PluginModule {
public:
    PluginModule(/* args */) = default;

    virtual ~PluginModule() = default;

    /**
     * 初始化模块
     * pModuleName: 模块名称(目录路径)，要求所属插件都放到这个目录
     * pInterfaceName: 各插件导出的获取实例接口名称，要求所属插件都统一导出相同名称
    */
    bool Init(const char *pModuleName, const char *pInterfaceName);

    // 释放各模块
    bool UInit();

    // 启动模块
    bool Start();

    // 停止模块
    bool Stop();

    // void SetMemPool(IMemoryPool *pMemPool);  // 内存池

    void SetDataQueue(Queue<ProtocolDataVar *> *pQueue);   //队列

    // 处理数据
    int ProcessData(ProtocolDataVar *pData);

    // 释放数据
    int ReleaseData(ProtocolDataVar *pData);

private:

#ifdef INLINE
    PluginInliner<GetPluginInterface, PluginImpl *> m_libs;
#else
    PluginLoader<GetPluginInterface, PluginImpl *> m_libs;
#endif
    std::vector<PluginImpl *> m_vClasses;       //端口采集对象集合
    Queue<ProtocolDataVar *> *pQueue_ = nullptr;//采集数据缓冲区
};

