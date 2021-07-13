
#pragma once

#include "PluginImpl.h"


class PrintProcessor : public PluginImpl {
public:
    // 获取插件名称
    const char *Name() override;

    bool Start() override;

    bool Stop() override;

    bool Init() override;

    bool UInit() override;


    // ==================加工类别插件接口==================
    //将数据交给由插件处理，处理正常需返回0，如果返回值>0，将不进入下一处理流程，直接在本流程结束。(计算、告警、扩展处理插件使用该返回值)
    int ProcessData(ProtocolDataVar *pData) override;
};

