
#include "GpsCollector.h"

#include <iostream>


extern "C" void *Instance() { return new GpsCollector; }


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

            std::cout << "生成经度" << pDataX << std::endl;
            pQueue_->push(pDataX);


            auto *pDataY = new ProtocolDataVar{
                    .name = std::string("北纬"),
                    .unit = std::string("单位：无"),
                    .group = std::string("gps"),
                    .source = std::string(Name()),
                    .getTime = (uint64_t) time(nullptr),
                    .data = "23°9'36\""
            };

            std::cout << "生成纬度" << pDataY << std::endl;
            pQueue_->push(pDataY);
        }

        // 线程已经退出

    });

    return true;
}

bool GpsCollector::Stop() {

    if (run_flag_) {
        run_flag_ = false;
        thread_->join();
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
    std::cout << "释放经纬" << pData << std::endl;
    delete pData;
    return 0;
}

void GpsCollector::SetDataQueue(std::queue<ProtocolDataVar *> *pQueue) {
    pQueue_ = pQueue;
}