cmake_minimum_required(VERSION 3.10)
project(restudycpp)


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
# -g -fsanitize=address开启内存泄漏检测功能，用-fno-omit-frame-pointer编译，以得到更容易理解stack trace。
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
########################


########################
# 链接选项 -rdynamic 针对可执行文件，用来通知链接器将所有符号导出到动态符号表中(默认情况下符号只会从共享库中导出)
# 在gcc中 -rdynamic 与 -Wl,-E 和 -Wl,--export-dynamic 的作用等价。
# 使用场景：bin需要打印backtrace时(可看到更详细的符号信息);.so反向调用bin时(extern); => 总之就是针对bin
# -ldl 链接选项针对dlopen, dlerror, dlsym, dlclose等动态库使用接口，使用-rdynamic的场景经常都会用到-ldl
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl -rdynamic")
########################

########################################################################################################################

if (WIN32)
    message("编译 windows")

elseif (DEFINED ENV{SDK_PATH})
    # 找到SDK_PATH环境变量 说明是交叉编译环境
    message("交叉编译 am335x")

else () #elseif (DEFINED ENV{WSLENV})
    message("交叉编译 wsl linux")

endif ()

########################################################################################################################

file(GLOB directorys RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach (directory ${directorys})
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${directory})
        if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${directory}/CMakeLists.txt)
            message("${directory}")
            add_subdirectory(${directory})
        endif ()
    endif ()
endforeach ()


add_subdirectory(plugin-framework/02plugin-queue)
#add_subdirectory(explicit-implicit)
#add_subdirectory(factory-mode)
#add_subdirectory(malloc-class-init)
#add_subdirectory(smart-pointer) # 智能指针
#add_subdirectory(value-move) # 右值与移动
#add_subdirectory(exception) # 抛出异常
#add_subdirectory(regex)
#add_subdirectory(lazy-evaluation) # 惰性求值
#add_subdirectory(thread-future)
#add_subdirectory(numeric)
#add_subdirectory(container) # 容器汇编
#add_subdirectory(variable-args)
