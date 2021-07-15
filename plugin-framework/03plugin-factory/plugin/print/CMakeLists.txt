
# 生产类插件
if (STATIC_LINK_MODE MATCHES OFF)
    add_library(PrintProcessor SHARED PrintProcessor.cpp)
    target_link_libraries(PrintProcessor PRIVATE -fPIC)
else ()
    add_library(PrintProcessor STATIC PrintProcessor.cpp)
endif ()


# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(PrintProcessor PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../Processor)
