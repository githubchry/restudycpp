
# 生产类插件

if (STATIC_LINK_MODE MATCHES OFF)
    add_library(GpsCollector SHARED GpsCollector.cpp)
    target_link_libraries(GpsCollector PRIVATE -fPIC)
else ()
    add_library(GpsCollector STATIC GpsCollector.cpp)
endif ()

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(GpsCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../Collector)

########################################################################################################################

# 编译调试版本：增加-g参数，输出到debug目录并重命名
if (STATIC_LINK_MODE MATCHES OFF)
    add_library(GpsCollector_debug SHARED GpsCollector.cpp)
    target_link_libraries(GpsCollector_debug PRIVATE -fPIC)
else ()
    add_library(GpsCollector_debug STATIC GpsCollector.cpp)
endif ()

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(GpsCollector_debug PROPERTIES COMPILE_FLAGS -g)
set_target_properties(GpsCollector_debug PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../debug/Collector)
set_target_properties(GpsCollector_debug PROPERTIES OUTPUT_NAME GpsCollector)
