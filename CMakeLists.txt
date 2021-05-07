cmake_minimum_required(VERSION 3.10)
project(restudycpp)

add_subdirectory(plugin-framework/02plugin-queue)
add_subdirectory(plugin-framework/03plugin-factory)

add_subdirectory(explicit-implicit)
add_subdirectory(factory-mode)
add_subdirectory(malloc-class-init)
add_subdirectory(smart-pointer)
