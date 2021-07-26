
# 生产类插件
if (STATIC_LINK_MODE MATCHES OFF)
    add_library(PrintProcessor SHARED PrintProcessor.cpp)
    target_link_libraries(PrintProcessor PRIVATE -fPIC)
else ()
    add_library(PrintProcessor STATIC PrintProcessor.cpp)
endif ()

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(PrintProcessor PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../Processor)

########################################################################################################################

# 编译调试版本：增加-g参数，输出到debug目录并重命名
if (STATIC_LINK_MODE MATCHES OFF)
    add_library(PrintProcessor_debug SHARED PrintProcessor.cpp)
    target_link_libraries(PrintProcessor_debug PRIVATE -fPIC)
else ()
    add_library(PrintProcessor_debug STATIC PrintProcessor.cpp)
endif ()

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(PrintProcessor_debug PROPERTIES COMPILE_FLAGS -g)
set_target_properties(PrintProcessor_debug PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../debug/Processor)
set_target_properties(PrintProcessor_debug PROPERTIES OUTPUT_NAME PrintProcessor)