cmake_minimum_required(VERSION 3.10)
project(plugin-factory)

########################
# 打开所有编译告警
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
########################


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
# 链接选项 -rdynamic 针对可执行文件，用来通知链接器将所有符号导出到动态符号表中(默认情况下符号只会从共享库中导出)
# 在gcc中 -rdynamic 与 -Wl,-E 和 -Wl,--export-dynamic 的作用等价。
# 使用场景：bin需要打印backtrace时(可看到更详细的符号信息);.so反向调用bin时(extern); => 总之就是针对bin
# -ldl 链接选项针对dlopen, dlerror, dlsym, dlclose等动态库使用接口，使用-rdynamic的场景经常都会用到-ldl
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl -rdynamic")
########################

########################################################################################################################

########################
# 内存泄漏检查工具 asan     cmake .. -DCMAKE_BUILD_TYPE=Debug
# -g -fsanitize=address开启内存泄漏检测功能，用-fno-omit-frame-pointer编译，以得到更容易理解stack trace。
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address -fno-omit-frame-pointer -ldl -lpthread -lrt")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fsanitize=address -fno-omit-frame-pointer -ldl -lpthread -lrt")
# 有些工具链的asan库可能会依赖： -ldl -lpthread -lrt
########################


########################
# 覆盖率检查工具 gcov      cmake .. -DCMAKE_BUILD_TYPE=Debug
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
########################


########################
# 性能测试工具 gprof     cmake .. -DCMAKE_BUILD_TYPE=Debug
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -pg")
# 无法测试动态加载库(dlopen)内部的代码 所以要求在静态链接模式下进行
########################


########################################################################################################################
# 增加编译选项：使能日志库
# cmake .. -DENABLE_ZLOG=ON
option(ENABLE_ZLOG "enable zlog" OFF)
if (ENABLE_ZLOG MATCHES ON)
    add_definitions(-DENABLE_LOG)
endif ()

# 增加编译选项：默认动态加载模式 可选静态链接模式
# cmake .. -DSTATIC_LINK_MODE=ON
option(STATIC_LINK_MODE "static link mode" OFF)
add_definitions(-DDYNAMIC_LOAD_MODE)  # 定义宏让代码判断当前模式：动态加载模式
if (STATIC_LINK_MODE MATCHES ON)
    message("plugin-factory static link mode: ${STATIC_LINK_MODE}")
    remove_definitions(-DDYNAMIC_LOAD_MODE) # 静态链接模式：删除预定义的宏
    include_directories(plugin)
endif ()

# 依赖库路径
include_directories(/mnt/d/codes/new-root/wsl/include)
link_directories(/mnt/d/codes/new-root/wsl/lib)

# 公用头文件
include_directories(impl)

# 插件源码目录：各插件根据STATIC_LINK_MODE编译选项和DYNAMIC_LOAD_MODE宏的定义情况来编译输出静态还是动态库
add_subdirectory(plugin)

# 主进程
include_directories(factory/manager)
include_directories(factory/worker)

aux_source_directory(factory/manager SOURCE_MANAGER_FILES)
aux_source_directory(factory/worker SOURCE_WORKER_FILES)

########################################################################################################################

set(output_name main)
list(APPEND source_files factory/main.cc ${SOURCE_MANAGER_FILES} ${SOURCE_WORKER_FILES})

# 分别编译release和debug版本，仅存在-g参数区别
set(build_types debug;release)
foreach (build_type IN LISTS build_types)
    add_executable(${output_name}_${build_type} ${source_files})

    set_target_properties(${output_name}_${build_type} PROPERTIES OUTPUT_NAME ${output_name})
    if (${build_type} MATCHES debug)
        set_target_properties(${output_name}_${build_type} PROPERTIES COMPILE_FLAGS -g)
        set_target_properties(${output_name}_${build_type} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${build_type})
    endif ()

    target_link_libraries(${output_name}_${build_type} PRIVATE pthread dl zlog)
    if (STATIC_LINK_MODE MATCHES ON)
        # 静态链接模式：将用到的插件逐个链接上去
        target_link_libraries(${output_name}_${build_type} PRIVATE TimeCollector PrintProcessor GpsCollector -rdynamic)
    endif ()
endforeach ()