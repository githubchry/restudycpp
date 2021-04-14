#pragma once

class PluginImpl
{
private:
    /* data */
public:
    PluginImpl(/* args */) {}
    virtual ~PluginImpl() {}
    virtual int Name() = 0;
};

typedef PluginImpl *GetPluginInterface();

