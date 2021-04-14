
#include "PluginTestObject.h"
#include <iostream>

extern "C" void *Instance() { return new PluginTestObject; }

int PluginTestObject::Name()
{
    std::cout << "class name:PluginTestObject   function name:Name()" << std::endl;
    return 0;
}