#include "collector.h"

#include <iostream>

#include <time.h>

extern "C" void *Instance() { return new Collector; }

const char *Collector::Name()
{
    return "Collector";
}

bool Collector::Start()
{
    isRuning = true;

    auto func = [this]() {
        while (isRuning)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            ProtocolDataVar *pData = new ProtocolDataVar{
                .name = std::string("time"),
                .unit = std::string("unit"),
                .group = std::string("group"),
                .source = std::string("source"),
                .getTime = (uint64_t)time(NULL),
            };

            pQueue_->push(pData);
        }
    };

    thread_ = std::thread(func);

    return true;
}

bool Collector::Stop()
{
    isRuning = false;
    thread_.join();

    return true;
}

int Collector::ReleaseData(ProtocolDataVar *pData)
{
    delete pData;
    pData = nullptr;
    return 0;
}

void Collector::SetDataQueue(std::queue<ProtocolDataVar *> *pQueue)
{
    pQueue_ = pQueue;
    return;
}