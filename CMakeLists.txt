cmake_minimum_required(VERSION 3.18)
project(hpx_demo CXX)

find_package(HPX REQUIRED)

add_executable(hello_hpx hello_hpx.cpp)
target_link_libraries(hello_hpx HPX::hpx HPX::wrap_main)

add_executable(collective_demo collective_demo.cpp)
target_link_libraries(collective_demo HPX::hpx HPX::wrap_main)

add_executable(matrix_multiplication_demo matrix_multiplication_demo.cpp)
target_link_libraries(matrix_multiplication_demo HPX::hpx HPX::wrap_main)
