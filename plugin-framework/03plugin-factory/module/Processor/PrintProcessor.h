
#pragma once

#include "PluginImpl.h"


class PrintProcessor : public PluginImpl {

private:

public:
    // 获取插件名称
    const char *Name() override;

    bool Start() override;

    bool Stop() override;

    bool Init() override;

    bool UInit() override;

    // ==================加工类别插件接口==================
    int ProcessData(ProtocolDataVar *pData) override;
};

