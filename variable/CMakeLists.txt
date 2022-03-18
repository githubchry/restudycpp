cmake_minimum_required(VERSION 3.10)
project(variable-args)

add_executable(args     args.cc)


# variant里面使用了c++17特性
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_executable(variant  variant.cc)