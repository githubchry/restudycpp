
#include <Processor.h>
#include <Collector.h>
#include "DataWorker.h"

DataWorker::~DataWorker() {

    Stop();
}

DataWorker::DataWorker(std::queue<ProtocolDataVar *> *pQueue) : pQueue_(pQueue) {

}

bool DataWorker::Start() {
    run_flag_ = true;
    thread_ = std::make_shared<std::thread>([this]() {

        while (run_flag_) {

            if (pQueue_->empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            ProtocolDataVar *pData = pQueue_->front();

            Processor::Instance().ProcessData(pData);

            Collector::Instance().ReleaseData(pData);

            pQueue_->pop();

        }

        // 线程已经退出

    });

    return true;
}

bool DataWorker::Stop() {

    if (run_flag_) {
        run_flag_ = false;
        thread_->join();

        while (!pQueue_->empty()) {
            Collector::Instance().ReleaseData(pQueue_->front());
            pQueue_->pop();
        }
    }


    return true;
}
