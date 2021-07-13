include_directories(../../impl)

# 生产类插件
add_library(GpsCollector SHARED GpsCollector.cpp)

target_link_libraries(GpsCollector PRIVATE -fPIC)

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(GpsCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../Collector)
