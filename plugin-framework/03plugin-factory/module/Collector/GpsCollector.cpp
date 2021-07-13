
#include "GpsCollector.h"

#include <iostream>


#ifndef INLINE
extern "C" void *Instance() { return new GpsCollector; }
#endif


const char *GpsCollector::Name() {
    return "GpsCollector";
}


bool GpsCollector::Start() {
    run_flag_ = true;
    thread_ = std::make_shared<std::thread>([this]() {

        while (run_flag_) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            auto *pDataX = new ProtocolDataVar{
                    .name = std::string("东经"),
                    .unit = std::string("单位：无"),
                    .group = std::string("gps"),
                    .source = std::string(Name()),
                    .getTime = (uint64_t) time(nullptr),
                    .data = "120°25'36\""
            };

            log_debug("生成经度:%p\n", pDataX);
            if (false == pQueue_->push(pDataX)){
                log_error("添加经度到队列失败:%p\n", pDataX);
                ReleaseData(pDataX);
            }

            auto *pDataY = new ProtocolDataVar{
                    .name = std::string("北纬"),
                    .unit = std::string("单位：无"),
                    .group = std::string("gps"),
                    .source = std::string(Name()),
                    .getTime = (uint64_t) time(nullptr),
                    .data = "23°9'36\""
            };

            log_debug("生成纬度:%p\n", pDataY);
            if (false == pQueue_->push(pDataY)){
                log_error("添加纬度到队列失败:%p\n", pDataY);
                ReleaseData(pDataY);
            }
        }

        // 线程已经退出

    });

#ifdef __linux__
    // 查看线程占用情况：top -Hp 进程PID -d1
    pthread_setname_np(thread_->native_handle(), Name());
#endif

    return true;
}

bool GpsCollector::Stop() {

    if (run_flag_) {
        run_flag_ = false;
        if(thread_->joinable()){
            thread_->join();
        }
    }

    return true;
}


bool GpsCollector::Init() {
    return true;
}


bool GpsCollector::UInit() {
    return true;
}


int GpsCollector::ReleaseData(ProtocolDataVar *pData) {
    log_debug("释放经纬:%p\n", pData);
    delete pData;
    return 0;
}

void GpsCollector::SetDataQueue(Queue<ProtocolDataVar *> *pQueue) {
    pQueue_ = pQueue;
}
