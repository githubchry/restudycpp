
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

            log_debug("生成时间:%p\n", pData);

            if (false == pQueue_->push(pData)){
                log_error("添加时间到队列失败:%p\n", pData);
                ReleaseData(pData);
            }
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
    log_debug("释放时间:%p\n", pData);
    delete pData;
    return 0;
}

void TimeCollector::SetDataQueue(Queue<ProtocolDataVar *> *pQueue) {
    pQueue_ = pQueue;
}