

#pragma once

#include <dlfcn.h> // dlopen, dlerror, dlsym, dlclose
#include <dirent.h> // opendir, readdir
#include <iostream>

template<class F, class T>
class PluginLoader {
public:
    PluginLoader() = default;

    ~PluginLoader();

    T PluginLoad(const char *pLibPath, const char *pInstanceName);

    void PluginModLoad(const char *pModPath, const char *pInstanceName, std::vector<T> &vClass);

private:
    std::vector<void *> m_vHandle;
};


template<class F, class T>
PluginLoader<F, T>::~PluginLoader() {

    for (auto iter : m_vHandle) {
        dlclose(iter);
    }
}

// 加载一个插件, 管理动态库和实例的生命周期
template<class F, class T>
T PluginLoader<F, T>::PluginLoad(const char *pLibPath, const char *pInstanceName) {
    T ptr_ = NULL;
    // 加载插件
    void *handle_ = dlopen(pLibPath, RTLD_LAZY);
    if (!handle_) {
        std::cout << pLibPath << " dlopen error:" << dlerror() << std::endl;
        return NULL;
    }

    m_vHandle.push_back(handle_);

    // 获取实例
    F *GetInstance = (F *) dlsym(handle_, pInstanceName);
    if (!GetInstance) {
        std::cout << pLibPath << " dlsym " << pInstanceName << " error:" << dlerror() << std::endl;
        return NULL;
    }

    ptr_ = GetInstance();

    if (!ptr_) {
        std::cout << pLibPath << " " << pInstanceName << " GetInstance error:" << dlerror() << std::endl;
        return NULL;
    }


    return ptr_;
}

// 加载一个模块（插件目录），仅管理动态库的生命周期
template<class F, class T>
void PluginLoader<F, T>::PluginModLoad(const char *pModPath, const char *pInstanceName, std::vector<T> &vClass) {

    DIR *dir = opendir(pModPath);
    if (!dir) {
        std::cout << "无法打开目录：" << pModPath << std::endl;
        return;
    }
    dirent *p = nullptr;
    while ((p = readdir(dir)) != nullptr) {
        if (p->d_name[0] != '.') {
            std::string libPath = pModPath + std::string("/") + p->d_name;
            //加载库
            T cls = PluginLoad(libPath.c_str(), pInstanceName);
            if (cls) {
                vClass.push_back(cls);
            } else {
                std::cout << "无法获取实例：" << pModPath << std::endl;
            }
        }
    }
    closedir(dir);
}