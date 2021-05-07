
#include <unistd.h>     //sleep
#include <thread>
#include <DataWorker.h>
#include "Collector.h"
#include "Processor.h"

int main() {

    auto *queue = new std::queue<ProtocolDataVar *>;

    Collector::Instance().SetDataQueue(queue);

    Collector::Instance().Init();
    Processor::Instance().Init();

    DataWorker worker(queue);
    worker.Start();

    Collector::Instance().Start();
    Processor::Instance().Start();

    //测试5秒后退出
    sleep(5);
    std::cout<<"退出"<<std::endl;

    Collector::Instance().Stop();
    Processor::Instance().Stop();

    worker.Stop();

    Processor::Instance().UInit();
    Collector::Instance().UInit();

    delete queue;

    return 0;
}