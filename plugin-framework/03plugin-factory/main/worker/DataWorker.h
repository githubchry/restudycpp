#pragma once

#include <atomic>
#include <thread>
#include "PluginImpl.h"

class DataWorker {
public:
    explicit DataWorker(std::queue<ProtocolDataVar *> *pQueue);

    ~DataWorker();
    
    bool Start();

    bool Stop();

private:
    /* data */
    std::queue<ProtocolDataVar *> *pQueue_ = nullptr;
    std::atomic<bool> run_flag_{false};
    std::shared_ptr<std::thread> thread_;
};

