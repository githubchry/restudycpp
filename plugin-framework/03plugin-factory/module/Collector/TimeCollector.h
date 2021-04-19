#pragma once

#include "PluginImpl.h"
#include <thread>
#include <queue>
#include <atomic>

class TimeCollector : public PluginImpl {

private:
    /* data */
    std::queue<ProtocolDataVar *> *pQueue_ = nullptr;
    std::atomic<bool> run_flag_{};
    std::shared_ptr<std::thread> thread_;

public:
    // TimeCollector::TimeCollector() = default;
    // 获取插件名称
    const char *Name() override;

    bool Start() override;

    bool Stop() override;

    bool Init() override;

    bool UInit() override;

    // ==================生产类别插件接口==================
    int ReleaseData(ProtocolDataVar *pData) override;

    void SetDataQueue(std::queue<ProtocolDataVar *> *pQueue) override;
};


