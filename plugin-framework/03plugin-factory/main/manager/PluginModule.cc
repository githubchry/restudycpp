#include "PluginModule.h"

bool PluginModule::Init(const char *pModuleName, const char *pInterfaceName) {

    m_libs.PluginModuleLoad(pModuleName, pInterfaceName, m_vClasses);
    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->SetDataQueue(pQueue_);
    }

    return false;
}

bool PluginModule::UInit() {

    // 反初始化插件依赖信息
    for (auto iter : m_vClasses) {
        delete iter;
    }

    return false;
}

bool PluginModule::Start() {
    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->Start();
    }

    return false;
}


bool PluginModule::Stop() {
    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->Stop();
    }

    return false;
}

void PluginModule::SetDataQueue(Queue<ProtocolDataVar *> *pQueue) {
    pQueue_ = pQueue;
}

int PluginModule::ProcessData(ProtocolDataVar *pData) {

    // 初始化插件依赖信息
    for (auto iter : m_vClasses) {
        iter->ProcessData(pData);
    }

    return 0;
}

int PluginModule::ReleaseData(ProtocolDataVar *pData) {

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

