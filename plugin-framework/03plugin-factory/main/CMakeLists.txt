cmake_minimum_required(VERSION 3.2)

project(main)

include_directories(./manager)
include_directories(./worker)
include_directories(../impl)

aux_source_directory(./manager SOURCE_MANAGER_FILES)
aux_source_directory(./worker SOURCE_WORKER_FILES)

add_executable(main main.cc ${SOURCE_MANAGER_FILES} ${SOURCE_WORKER_FILES} worker/DataWorker.cpp worker/DataWorker.h)
target_link_libraries(main PRIVATE pthread dl)
