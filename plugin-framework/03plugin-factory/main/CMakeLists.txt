
include_directories(../impl)
include_directories(./manager)
include_directories(./worker)

aux_source_directory(./manager SOURCE_MANAGER_FILES)
aux_source_directory(./worker SOURCE_WORKER_FILES)

add_executable(plugin-factory main.cc ${SOURCE_MANAGER_FILES} ${SOURCE_WORKER_FILES})
target_link_libraries(plugin-factory PRIVATE pthread dl zlog)
# 指定可执行文件输出目录  默认当前CMakeLists.txt所在目录$CMAKE_SOURCE_DIR
set_target_properties(plugin-factory PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)
