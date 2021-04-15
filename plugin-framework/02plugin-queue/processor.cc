#include "processor.h"

#include <iostream>

extern "C" void *Instance() { return new Processor; }

const char *Processor::Name()
{
    return "Processor";
}

int Processor::ProcessData(ProtocolDataVar *pData)
{
    if (pData)
    {
        std::cout << "ProcessData: " << pData->name  << pData->getTime << std::endl;
    }

    return 0;
}
