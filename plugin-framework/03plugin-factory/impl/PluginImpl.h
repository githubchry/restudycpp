
#pragma once

#include <string>
#include <queue>

struct ProtocolDataVar {
    std::string name;   //名字
    std::string unit;   //单位
    std::string group;  //数据所属分组
    std::string source; //数据从哪个模块来
    uint64_t getTime;   //产生时间

    std::string data; //数据 自行扩展
};

class PluginImpl {
private:
    /* data */
public:
    PluginImpl(/* args */) = default;

    virtual ~PluginImpl() = default;

    // =====================公共接口======================
    // 获取插件名称
    virtual const char *Name() = 0;

    /*
	 * 初始化以及释放接口
	 */
    virtual bool Init() = 0;

    virtual bool UInit() = 0;

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
    virtual void SetDataQueue(std::queue<ProtocolDataVar *> *pQueue) {};

    virtual int ReleaseData(ProtocolDataVar *pData) { return 0; };

    // ==================加工类别插件接口==================
    virtual int ProcessData(ProtocolDataVar *pData) { return 0; };
};

typedef PluginImpl *GetPluginInterface();
