#include "PluginMod.h"

bool PluginMod::Init(const char *pModuleName, const char *pInterfaceName) {

    m_libs.PluginModLoad(pModuleName, pInterfaceName, m_vClasses);

    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        std::cout << iter << std::endl;
        iter->SetDataQueue(pQueue_);
    }

    return false;
}

bool PluginMod::Start() {

    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->Start();
    }

    return false;
}

void PluginMod::SetDataQueue(std::queue<ProtocolDataVar *> *pQueue) {
    pQueue_ = pQueue;
}

int PluginMod::ProcessData(ProtocolDataVar *pData) {

    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->ProcessData(pData);
    }

    return 0;
}

bool PluginMod::UInit() {

    // 初始化插件依赖信息

    std::cout << "m_vClasses.size()" << m_vClasses.size() << std::endl;

    for (auto iter : m_vClasses) {
        std::cout << iter << std::endl;
        delete iter;
    }

    return false;
}

bool PluginMod::Stop() {

    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->Stop();
    }

    return false;
}

int PluginMod::ReleaseData(ProtocolDataVar *pData) {

    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        //必须检测这个是不是插件生成的数据 避免重复释放导致崩溃
        if (pData->source == iter->Name()) {
            iter->ReleaseData(pData);
            break;
        }
    }

    return 0;
}
