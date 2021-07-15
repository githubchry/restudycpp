
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
    if (stack_frame_string_buffer == NULL) {
        perror("backtrace_symbols");
        exit(-1);
    }

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
    signal(SIGSEGV, signal_handler);

#ifdef ENABLE_LOG
    scoped_zlog zlog_init("zlog.conf", "my_cat");
#endif

#ifdef DYNAMIC_LOAD_MODE
    log_debug("动态加载模式\n");
#else
    log_debug("静态加载模式\n");
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