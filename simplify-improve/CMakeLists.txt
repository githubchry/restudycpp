cmake_minimum_required(VERSION 3.10)
project(simplify-improve)

# 指定为C++17 版本
set(CMAKE_CXX_STANDARD 17)

file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)

foreach (file ${files})
    # 正则匹配 04future.cc => 04future
    string(REGEX REPLACE "(.+)\\..*" "\\1" target_name ${file})

    message("${file} => ${target_name}")

    add_executable(${target_name} ${file})
endforeach ()
