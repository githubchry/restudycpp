cmake_minimum_required(VERSION 3.10)
project(future)

add_executable(01mutex 01mutex.cc)
add_executable(02condition_variable 02condition_variable.cc)
add_executable(03condition_variable_wait_for_time_bug 03condition_variable_wait_for_time_bug.cc)
add_executable(04future 04future.cc)
