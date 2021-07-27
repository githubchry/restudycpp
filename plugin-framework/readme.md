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



## [03plugin-factory](https://github.com/githubchry/plugin-factory)

1. 对可能用到的插件进行分类， 比如采集类插件， 加工类插件……

2. 根据类别整合所有功能， 虚拟到一个基类上

3. 一个类别的所有插件作为一个模块， 模块对其插件进行统一管理

   



# 错误排查

**现象**

```
dlsym返回null，打印dlerror()：/mnt/d/xxxx/protocols/libsysinfo.so: undefined symbol: _ZTI7IPlugin
```

**分析**

用`nm -D`查看so是否存在符号

```
nm -D libsysinfo.so | grep _ZTI7IPlugin
输出如下：
                 U _ZTI7IPlugin


```

用`ldd -r`查看so链接信息

```
ldd -r libsysinfo.so
输出如下
        linux-vdso.so.1 (0x00007ffd9cdc6000)
        libasan.so.5 => /lib/x86_64-linux-gnu/libasan.so.5 (0x00007f4a82169000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f4a81f88000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f4a81f6d000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f4a81d7b000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f4a81d75000)
        librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f4a81d6a000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f4a81d45000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f4a81bf6000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f4a82bb2000)
undefined symbol: _ZTI7IPlugin  (./libsysinfo.so)
undefined symbol: _ZN7IPlugin16SetHardwareParamEPv      (./libsysinfo.so)
undefined symbol: _ZN7IPlugin14SetSystemParamEPv        (./libsysinfo.so)
```

**原因**

sysinfo里面以IPlugin为基类实现了一个插件，出现`_ZTI7IPlugin`应该是因为这个基类存在虚函数没完全实现。

**解决过程**

然后在代码里面override了SetHardwareParam和SetSystemParam两个函数，并使用代码编辑器生成了默认的实现：

```
bool SysInfoPluginImpl::SetHardwareParam(void *pHardwareConfig) {
    return IPlugin::SetHardwareParam(pHardwareConfig);
}

bool SysInfoPluginImpl::SetSystemParam(void *pSystemConfig) {
    return IPlugin::SetSystemParam(pSystemConfig);
}
```

用`ldd -r`查看so链接信息

```
ldd -r libsysinfo.so
输出如下
        linux-vdso.so.1 (0x00007ffc8712e000)
        libasan.so.5 => /lib/x86_64-linux-gnu/libasan.so.5 (0x00007fb4b96ac000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fb4b94cb000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fb4b94b0000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fb4b92be000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007fb4b92b8000)
        librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007fb4b92ad000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fb4b9288000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fb4b9139000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fb4ba0f5000)
undefined symbol: _ZN7IPlugin16SetHardwareParamEPv      (./libsysinfo.so)
undefined symbol: _ZN7IPlugin14SetSystemParamEPv        (./libsysinfo.so)
```

`_ZTI7IPlugin`错误没了， 但依旧存在`SetHardwareParam`和`SetSystemParam`找不到，原因很明显，这两的确是没有实现，上面不应该用编辑器默认的写法，应该改成：

```
bool SysInfoPluginImpl::SetHardwareParam(void *pHardwareConfig) {
    return false;
}

bool SysInfoPluginImpl::SetSystemParam(void *pSystemConfig) {
    return false;
}

```

至此， 问题解决。



**现象二**

```
ldd -r libsysinfo.so
输出如下
        linux-vdso.so.1 (0x00007ffd9cdc6000)
        libasan.so.5 => /lib/x86_64-linux-gnu/libasan.so.5 (0x00007f4a82169000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f4a81f88000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f4a81f6d000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f4a81d7b000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f4a81d75000)
        librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f4a81d6a000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f4a81d45000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f4a81bf6000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f4a82bb2000)
undefined symbol: _ZTI7IPlugin  (./libsysinfo.so)
undefined symbol: _ZTI7IPlugin  (./libsysinfo.so)
```

### ** 解决**

```
修改虚类IPlugin，检查虚类的方法，定义成纯虚函数或者增加默认实现
```



