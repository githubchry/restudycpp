[c++中如何为一个程序写扩展?](https://www.zhihu.com/question/52538590/answer/137136003)

## 01plugin-demo

1. 写一个接口类，内部都是纯虚函数，用作定义对外接口。 - PluginImpl.h

2. 写一个实例类，继承实现这个接口。这个类不用导出。 - PluginTestObject.cpp/PluginTestObject.h

3. 导出一个C函数Instance如下。

   ```
   extern "C" void *Instance() { return new PluginTestObject; }
   ```

4. 使用插件者，通过文件名在运行时加载dll/so；

   ```
   // 加载插件
   void *handle = dlopen("./libto.so", RTLD_LAZY);
   if (!handle)
   {
   	std::cout << dlerror() << std::endl;
   	return 1;
   }
   ```

5. 使用插件者，通过字符串"getInstance"获取到函数指针；

   ```cpp
   typedef PluginImpl *GetPluginInterface();
   
   // 获取实例
   GetPluginInterface *GetInstance = (GetPluginInterface *)dlsym(handle, "Instance");
   if (!GetInstance)
   {
   	std::cout << "Error Instance function" << std::endl;
   	return 1;
   }
   ```

6. 运行函数指针，得到对象实例。然后就可以通过接口调用了。

   ```cpp
   PluginImpl *obj = GetInstance();
   if (obj)
   {
   	obj->Name();
   }
   ```

   至此，一个简陋的插件demo完成了。

## 02plugin-queue

1. 利用C++RAII特性机制，写一个PluginImplWrapper类进一步封装插件类。
2. 写两个插件分别实现采集、加工功能，共用一个虚拟基类，通过队列构成流水线



存在的问题：如果是生产消费模型，可能存在多个生产者和消费者，这时候的设计无法满足。



## 03plugin-factory

1. 对可能用到的插件进行分类， 比如采集类插件， 加工类插件……

2. 根据类别整合所有功能， 虚拟到一个基类上

3. 一个类别的所有插件作为一个模块， 模块对其插件进行统一管理

   





