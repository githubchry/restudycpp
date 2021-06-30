cmake_minimum_required(VERSION 3.10)
project(smart-pointer)


add_executable(00copy_constructor_assign    00copy_constructor_assign.cc)
add_executable(01template                   01template.cc)
add_executable(02operator                   02operator.cc)
add_executable(03prohibit_copy              03prohibit_copy.cc)
add_executable(04transfer_ownership         04transfer_ownership.cc)
add_executable(05move_ownership             05move_ownership.cc)
add_executable(06move_ownership_ex          06move_ownership_ex.cc)
add_executable(07reference_count            07reference_count.cc)
add_executable(08pointer_type_conversion    08pointer_type_conversion.cc)
