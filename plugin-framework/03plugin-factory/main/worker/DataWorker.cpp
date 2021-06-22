
#include <Processor.h>
#include <Collector.h>
#include "DataWorker.h"

DataWorker::~DataWorker() {

    Stop();
}

DataWorker::DataWorker(Queue<ProtocolDataVar *> *pQueue) : pQueue_(pQueue) {

}

bool DataWorker::Start() {
    run_flag_ = true;
    thread_ = std::make_shared<std::thread>([this]() {

        ProtocolDataVar *pData;
        while (run_flag_ && pQueue_->pop(pData)) {

            Processor::Instance().ProcessData(pData);

            Collector::Instance().ReleaseData(pData);
        }
    });

    return true;
}

bool DataWorker::Stop() {

    if (run_flag_) {
        run_flag_ = false;
        pQueue_->finished();
        thread_->join();

        ProtocolDataVar *pData;
        while (pQueue_->pop(pData)) {
            Collector::Instance().ReleaseData(pData);
        }
        pQueue_->quit();
    }


    return true;
}
