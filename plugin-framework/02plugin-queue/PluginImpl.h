// 三类插件 生产 加工 消费
/**
生产者producers
加工者processors
消费者consumers


produce
process
consume
*/

#include <string>
#include <queue>

struct ProtocolDataVar
{
    std::string name;   //名字
    std::string unit;   //单位
    std::string group;  //数据所属分组
    std::string source; //数据从哪个模块来
    uint64_t getTime;   //产生时间
};

#pragma once

class PluginImpl
{
private:
    /* data */
public:
    PluginImpl(/* args */) {}
    virtual ~PluginImpl() {}

    // =====================公共接口======================
    // 获取插件名称
    virtual const char *Name() = 0;
    
	/*
	 * 开启，停止模块运行
	 * 各模块可以根据自身是否需要开启线程来实现相应的操作 
	 */
	virtual bool Start() { return true; }
	virtual bool Stop() { return true; }


    // ==================采集类插件接口==================
    /**
     * 控制数据的生成和销毁，中间由加工类插件处理
    */
	virtual void SetDataQueue(std::queue<ProtocolDataVar*>* pQueue) {};
    virtual int ReleaseData(ProtocolDataVar *pData) { return 0; };


    // ==================加工类别插件接口==================
    virtual int ProcessData(ProtocolDataVar *pData) { return 0; };
};

typedef PluginImpl *GetPluginInterface();
