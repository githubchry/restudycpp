
#include <iostream>
#include "TimeCollector.h"


extern "C" void *Instance() { return new TimeCollector; }


const char *TimeCollector::Name() {
    return "TimeCollector";
}


bool TimeCollector::Start() {
    run_flag_ = true;
    thread_ = std::make_shared<std::thread>([this]() {

        while (run_flag_) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            auto *pData = new ProtocolDataVar{
                    .name = std::string(Name()),
                    .unit = std::string("单位：无"),
                    .group = std::string("group"),
                    .source = std::string(Name()),
                    .getTime = (uint64_t) time(nullptr),
            };

            std::cout << "生成时间" << pData << std::endl;
            pQueue_->push(pData);
        }

        // 线程已经退出

    });

    return true;
}

bool TimeCollector::Stop() {

    if (run_flag_) {
        run_flag_ = false;
        thread_->join();
    }

    return true;
}


bool TimeCollector::Init() {
    return true;
}


bool TimeCollector::UInit() {
    return true;
}


int TimeCollector::ReleaseData(ProtocolDataVar *pData) {
    std::cout << "释放时间" << pData << std::endl;
    delete pData;
    return 0;
}

void TimeCollector::SetDataQueue(std::queue<ProtocolDataVar *> *pQueue) {
    pQueue_ = pQueue;
}