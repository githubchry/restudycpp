cmake_minimum_required(VERSION 3.2)

project(module)

include_directories(../impl)

# 生产类插件
add_library(TimeCollector   SHARED Collector/TimeCollector.cpp)
add_library(GpsCollector   SHARED Collector/GpsCollector.cpp)

target_link_libraries(TimeCollector PRIVATE -fPIC)
target_link_libraries(GpsCollector PRIVATE -fPIC)

set_target_properties(TimeCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../main/Collector)
set_target_properties(GpsCollector PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../main/Collector)

# 加工类插件
add_library(PrintProcessor   SHARED Processor/PrintProcessor.cpp)

target_link_libraries(PrintProcessor PRIVATE -fPIC)

set_target_properties(PrintProcessor PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../main/Processor)
