include_directories(../impl)

# 生产类插件
add_library(TimeCollector   SHARED Collector/TimeCollector.cpp)
add_library(GpsCollector   SHARED Collector/GpsCollector.cpp)

target_link_libraries(TimeCollector PRIVATE -fPIC)
target_link_libraries(GpsCollector PRIVATE -fPIC)

# 指定库文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(TimeCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../Collector)
set_target_properties(GpsCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../Collector)

# 加工类插件
add_library(PrintProcessor   SHARED Processor/PrintProcessor.cpp)
target_link_libraries(PrintProcessor PRIVATE -fPIC)
set_target_properties(PrintProcessor PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../Processor)
