#pragma once

#include "PluginMod.h"
#include "singleton.h"

class Collector : public PluginMod {
SINGLETON_STACK_CTOR(Collector);
public:
    bool Init() {
        return PluginMod::Init("Collector", "Instance");
    }
};
