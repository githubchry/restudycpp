

#pragma once

#include <iostream>
#include <vector>
#include <cstring>

// 内联调试：直接将各源码内联到main里面
// 因为通过dlopen so动态库的方式很难调试（不支持gprof/backtrace），所以增加了内联调试模式
// 开发环境可以使用内联模式调试问题，稳定后在再使用插件模式进行测试和生产

#include "gps/GpsCollector.h"
#include "time/TimeCollector.h"
#include "print/PrintProcessor.h"

template<class F, class T>
class PluginLinker {
public:
    PluginLinker() = default;

    ~PluginLinker() = default;

    void PluginModuleLoad(const char *pModuleName, const char *pInstanceName, std::vector<T> &vClass);
};

// 内联插件模块并返回，插件实例的声明周期由调用者管理
template<class F, class T>
void PluginLinker<F, T>::PluginModuleLoad(const char *pModuleName, const char *pInstanceName, std::vector<T> &vClass) {
    // 这里 new 出来 push 到 vClass，后面调用者 Unint 时会执行 delete
    if (0 == strcmp(pModuleName, "Collector")){
        vClass.push_back(new GpsCollector);
        vClass.push_back(new TimeCollector);
    } else if (0 == strcmp(pModuleName, "Processor")) {
        vClass.push_back(new PrintProcessor);
    }
}