cmake_minimum_required(VERSION 3.10)
project(container)

# output_container.h的主要用途是打印容器内容，里面使用了c++17特性
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(01string 01string.cc)
add_executable(02vector 02vector.cc)
add_executable(03deque 03deque.cc)
add_executable(04list 04list.cc)
add_executable(05forward_list 05forward_list.cc)
add_executable(06queue 06queue.cc)
add_executable(07stack 07stack.cc)
add_executable(08func_obj 08func_obj.cc)
add_executable(09priority_queue 09priority_queue.cc)
add_executable(10associative_container 10associative_container.cc)
add_executable(11unordered_associative_container 11unordered_associative_container.cc)
add_executable(12array 12array.cc)
add_executable(map main.cc)