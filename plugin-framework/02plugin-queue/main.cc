#include "PluginImpl.h"
#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include <dlfcn.h> // dlopen, dlerror, dlsym, dlclose

template <typename T>
class PluginImplWrapper
{
public:
	explicit PluginImplWrapper(const char *lib_path, const char *instance)
	{
		// 加载插件
		handle_ = dlopen(lib_path, RTLD_LAZY);
		if (!handle_)
		{
			std::cout << dlerror() << std::endl;
		}

		// 获取实例
		GetPluginInterface *GetInstance = (GetPluginInterface *)dlsym(handle_, instance);
		if (!GetInstance)
		{
			std::cout << "Error Instance function" << std::endl;
		}

		ptr_ = GetInstance();

		if (!ptr_)
		{
			std::cout << "Error GetInstance" << std::endl;
		}
	}
	~PluginImplWrapper()
	{
		delete ptr_;
		if (handle_)
			dlclose(handle_);
	}

	T &operator*() const { return *ptr_; } //用 * 运算符解引用
	T *operator->() const { return ptr_; } //用 -> 运算符指向对象成员
	operator bool() const { return ptr_; } //像指针一样用在布尔表达式里

	T *get() const { return ptr_; }

private:
	void *handle_ = nullptr;
	T *ptr_ = nullptr;
};

int main()
{

	PluginImplWrapper<PluginImpl> collector("./libcollector.so", "Instance");
	PluginImplWrapper<PluginImpl> processor("./libprocessor.so", "Instance");

	std::queue<ProtocolDataVar *> queue;

	std::cout << collector->Name() << std::endl;
	collector->SetDataQueue(&queue);
	collector->Start();

	//测试5秒后退出
	bool isRunning = true;
	auto func = [](bool *isRunning) {std::this_thread::sleep_for(std::chrono::seconds(5)); *isRunning = false; };
	std::thread t1(func, &isRunning);

	while (isRunning)
	{
		if (queue.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		ProtocolDataVar *pData = queue.front();

		processor->ProcessData(pData);

		collector->ReleaseData(pData);

		queue.pop();
	}

	std::cout << queue.size() << std::endl;

	collector->Stop();

	std::cout << queue.size() << std::endl;

	while (!queue.empty())
	{
		collector->ReleaseData(queue.front());
		queue.pop();
	}

	t1.join();

	return 0;
}