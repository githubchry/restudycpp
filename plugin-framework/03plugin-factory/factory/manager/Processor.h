#pragma once

#include "PluginModule.h"
#include "singleton.h"

class Processor : public PluginModule {
SINGLETON_STACK_CTOR(Processor);
public:
    bool Init() {
        return PluginModule::Init("Processor", "Instance");
    }
};