cmake_minimum_required(VERSION 3.10)
project(plugin-factory)

########################
#有时我们的程序会定义一些暂时使用不上的功能和函数，虽然我们不使用这些功能和函数，但它们往往会浪费我们的ROM和RAM的空间。
#这在使用静态库时，体现的更为严重。有时，我们只使用了静态库仅有的几个功能，但是系统默认会自动把整个静态库全部链接到可执行程序中，造成可执行程序的大小大大增加。
#为了解决前面分析的问题，我们引入了以下的几个参数。
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -Wl,-gc-sections")
#GCC链接操作是以section作为最小的处理单元，只要一个section中的某个符号被引用，该section就会被加入到可执行程序中去。
#因此，GCC在编译时可以使用 -ffunction-sections和 -fdata-sections 将每个函数或符号创建为一个sections，其中每个sections名与function或data名保持一致。
#而在链接阶段， -Wl,–gc-sections 指示链接器去掉不用的section（其中-Wl, 表示后面的参数 -gc-sections 传递给链接器），这样就能减少最终的可执行程序的大小了。
########################


########################
#-fPIC 作用于编译阶段，告诉编译器产生与位置无关代码(Position-Independent Code)，则产生的代码中，没有绝对地址，全部使用相对地址，
#故而代码可以被加载器加载到内存的任意位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。
#[gcc编译参数-fPIC的一些问题](http://blog.sina.com.cn/s/blog_54f82cc201011op1.html)
#要求仅对共享库指定，如果对可执行文件指定可能会影响性能
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
########################


########################
# 内存泄漏检查工具 asan     cmake .. -DCMAKE_BUILD_TYPE=Debug
# -g -fsanitize=address开启内存泄漏检测功能，用-fno-omit-frame-pointer编译，以得到更容易理解stack trace。
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -fno-omit-frame-pointer -ldl -lpthread -lrt")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=address -fno-omit-frame-pointer -ldl -lpthread -lrt")
# 有些工具链的asan库可能会依赖： -ldl -lpthread -lrt
########################


########################
# 覆盖率检查工具 gcov      cmake .. -DCMAKE_BUILD_TYPE=Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
########################


########################################################################################################################
include_directories(/mnt/d/codes/new-root/wsl/include)
link_directories(/mnt/d/codes/new-root/wsl/lib)
link_directories(/mnt/d/codes/new-root/lib)

# 增加编译选项：默认动态加载模式 可选静态链接模式
# cmake .. -DSTATIC_LINK_MODE=ON
option(STATIC_LINK_MODE "static link mode" OFF)
add_definitions(-DDYNAMIC_LOAD_MODE)  # 定义宏让代码判断当前模式：动态加载模式
message("plugin-factory static link mode: ${STATIC_LINK_MODE}")


if (STATIC_LINK_MODE MATCHES ON)
remove_definitions(-DDYNAMIC_LOAD_MODE)
include_directories(plugin)
endif ()

include_directories(impl)   # 公用头文件
add_subdirectory(plugin)    # 插件源码目录

# 主进程
include_directories(factory/manager)
include_directories(factory/worker)

aux_source_directory(factory/manager SOURCE_MANAGER_FILES)
aux_source_directory(factory/worker SOURCE_WORKER_FILES)

add_executable(main factory/main.cc ${SOURCE_MANAGER_FILES} ${SOURCE_WORKER_FILES})

if (STATIC_LINK_MODE MATCHES OFF)
    target_link_libraries(main PRIVATE pthread dl zlog)
else ()
    target_link_libraries(main PRIVATE pthread dl zlog TimeCollector PrintProcessor GpsCollector -rdynamic)
endif ()


