
# 生产类插件

if (STATIC_LINK_MODE MATCHES OFF)
    add_library(GpsCollector SHARED GpsCollector.cpp)
    target_link_libraries(GpsCollector PRIVATE -fPIC)
else ()
    add_library(GpsCollector STATIC GpsCollector.cpp)
endif ()


# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(GpsCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../Collector)
