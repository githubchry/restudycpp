#pragma once

#include <atomic>
#include <thread>
#include "PluginImpl.h"

class DataWorker {
public:
    explicit DataWorker(Queue<ProtocolDataVar *> *pQueue);

    ~DataWorker();
    
    bool Start();

    bool Stop();

private:
    /* data */
    Queue<ProtocolDataVar *> *pQueue_ = nullptr;   // 非线程安全
    std::atomic<bool> run_flag_{false};
    std::shared_ptr<std::thread> thread_;
};

