#pragma once

#include "PluginModule.h"
#include "singleton.h"

class Collector : public PluginModule {
SINGLETON_STACK_CTOR(Collector);
public:
    bool Init() {
        return PluginModule::Init("Collector", "Instance");
    }
};
