#include "PluginImpl.h"
#include <iostream>
#include <dlfcn.h>


int main(int argc, char *argv[])
{

    // 加载插件
    void *handle = dlopen("./libto.so", RTLD_LAZY);
    if (!handle)
    {
        std::cout << dlerror() << std::endl;
        return 1;
    }

    // 获取实例
    GetPluginInterface *GetInstance = (GetPluginInterface *)dlsym(handle, "Instance");
    if (!GetInstance)
    {
        std::cout << "Error Instance function" << std::endl;
        return 1;
    }

    PluginImpl *obj = GetInstance();
    if (obj)
    {
        obj->Name();
    }

    delete obj;
    dlclose(handle);

    return 0;
}