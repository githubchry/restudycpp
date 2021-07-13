
#include <unistd.h>     //sleep
#include <DataWorker.h>

#include "Collector.h"
#include "Processor.h"

int main() {

#ifdef ENABLE_LOG
    scoped_zlog zlog_init("zlog.conf", "my_cat");
#endif

#ifdef INLINE
    log_debug("内联模式\n");
#else
    log_debug("插件模式\n");
#endif

    log_debug("debug\n");
    log_info("info\n");
    log_notice("notice\n");
    log_warn("warn\n");
    log_error("error\n");
    log_fatal("fatal\n");

    Queue<ProtocolDataVar *> queue(100);

    Collector::Instance().SetDataQueue(&queue);
    Collector::Instance().Init();   // 对该类别下各插件逐个设置 logger queue

    Processor::Instance().Init();

    DataWorker worker(&queue);
    worker.Start();

    Processor::Instance().Start();
    Collector::Instance().Start();

    //测试5秒后退出
    sleep(5);

    Collector::Instance().Stop();
    Processor::Instance().Stop();

    worker.Stop();

    Processor::Instance().UInit();
    Collector::Instance().UInit();

    return 0;
}