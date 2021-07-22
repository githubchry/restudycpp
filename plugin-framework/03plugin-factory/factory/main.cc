
#include <unistd.h>     //sleep
#include <DataWorker.h>

#include "Collector.h"
#include "Processor.h"

#include <signal.h>
#include <stdlib.h>
#include <execinfo.h>   // backtrace
#include <fstream>

// 定义保存栈帧的最大深度 根据项目复杂度定
#define STACK_FRAME_BUFFER_SIZE (int)128

void dump_backtrace() {
    // 定义栈帧缓冲区
    void *stack_frame_buffer[STACK_FRAME_BUFFER_SIZE];

    // 获取当前线程的栈帧
    int stack_frames_size = backtrace(stack_frame_buffer, STACK_FRAME_BUFFER_SIZE);

    // 将栈帧信息转化为字符串
    char **stack_frame_string_buffer = backtrace_symbols(stack_frame_buffer, stack_frames_size);
    assert_param(stack_frame_string_buffer);

    // 遍历打印栈帧信息
    for (int i = 0; i < stack_frames_size; i++) {
        log_fatal("[%02d] %s\n", i, stack_frame_string_buffer[i]);
    }

    // 释放栈帧信息字符串缓冲区
    free(stack_frame_string_buffer);
}

void dump_maps() {
    // std::string cmd = "cat /proc/" + std::to_string(getpid()) + "/maps";
    std::string cmd = "cat /proc/" + std::to_string(getpid()) + "/maps | grep r-xp";

    FILE *stream = popen(cmd.c_str(), "r");
    assert_param(stream);

    ssize_t read;
    char *line = nullptr;
    size_t len = 0;

    while ((read = getline(&line, &len, stream)) != -1) {
        log_fatal("%s", line);
    }

    pclose(stream);
}

void signal_handler(int signo) {
    log_fatal("\n=================>>>catch signal %d<<<=====================\n", signo);
    dump_backtrace();
    dump_maps();

#ifdef ENABLE_LOG
    zlog_fini();
    system("sync");
#endif

    // 恢复信号默认处理(SIG_DFL)并重新发送信号(raise)
    signal(signo, SIG_DFL);
    raise(signo);
}


int main() {
    // 捕获段错误信号SIGSEGV
    signal(SIGABRT, signal_handler);    // double-free abort 堆越界 assert
    signal(SIGSEGV, signal_handler);    // 非法内存访问错误
    signal(SIGBUS, signal_handler);     // 未对齐的数据访问

#ifdef ENABLE_LOG
    /* TODO:
     * 想法：是否需要把日志划分为运行日志，操作日志，错误日志？
     * 如果分开，连贯性没了
     * 如果不分，查找难度大
     *
     * 该问题代码无关，修改zlog配置文件即可实现，保留现有设计，平时写代码稍微注意一下打印类别即可
     */
    scoped_zlog zlog_init("zlog.conf", "my_cat");
#endif

#ifdef DYNAMIC_LOAD_MODE
    log_debug("动态加载模式\n");
#else
    log_debug("静态链接模式\n");
#endif

    log_debug("debug\n");
    log_info("info\n");
    log_notice("notice\n");
    log_warn("warn\n");
    log_error("error\n");
    log_fatal("fatal\n");

    /*
     * TODO: 队列的类型选择目前只有一种普通队列
     * 普通队列，满则堵塞
     * 环形队列，仅缓存最新N个数据，新的替换旧的
     * 优先级队列，紧急数据可能会插入到队列前面
     * 无锁队列，极端情况下，性能跟不上，可能需要使用CAS实现
     */
    Queue<ProtocolDataVar *> queue(100);

    // TODO: 除了队列，各插件通用的还有内存池，线程池，配置文件等

    /*
     * 主进程跑流水线任务，配置谁负责管理？BS结构还需要设计一个 webserver
     * main和webserver之间就需要进程间的通讯 => 配置更新实时生效
     *
     * 但是各种配置的读写肯定是在各插件的代码实现，难道webserver进程也要加载一次各插件？是否多此一举了？
     * 主进程同时负责webserver和woker？这样就省下了不少资源，也不用去搞进程间通信
     * 复杂度更高了吗？ 不复杂！进程间通信才复杂
     */


    /*
     * TODO: 插件类型一般不只这两种，可以从很多种层面划分
     * 串口 <==透传==> 网络
     *      串口读 ==上行==>  网络发
     *      网络收 ==下行==>  串口写
     *
     * 采集 => 计算 => 加工 => 存储 => 平台对接(上传)
     *                    => 报警 => 平台对接(上传)
     *
     * 需要根据需求自行设计插件类型PluginModule和流水线Worker
     */
    Collector::Instance().SetDataQueue(&queue);
    Collector::Instance().Init();

    Processor::Instance().Init();

    // TODO: worker不应该仅一个，起码两个保证数据上行和下行
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