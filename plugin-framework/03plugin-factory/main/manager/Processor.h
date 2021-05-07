#pragma once

#include "PluginMod.h"
#include "singleton.h"

class Processor : public PluginMod {
SINGLETON_STACK_CTOR(Processor);
public:
    bool Init() {
        return PluginMod::Init("Processor", "Instance");
    }
};