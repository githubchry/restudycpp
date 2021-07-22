
# 生产类插件
if (STATIC_LINK_MODE MATCHES OFF)
    add_library(TimeCollector SHARED TimeCollector.cpp)
    ########################
    #-fPIC 作用于编译阶段，告诉编译器产生与位置无关代码(Position-Independent Code)，则产生的代码中，没有绝对地址，全部使用相对地址，
    #故而代码可以被加载器加载到内存的任意位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。
    #[gcc编译参数-fPIC的一些问题](http://blog.sina.com.cn/s/blog_54f82cc201011op1.html)
    #要求仅对共享库指定，如果对可执行文件指定可能会影响性能
    ########################
    target_link_libraries(TimeCollector PRIVATE -fPIC)
else ()
    add_library(TimeCollector STATIC TimeCollector.cpp)
endif ()

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(TimeCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../Collector)
