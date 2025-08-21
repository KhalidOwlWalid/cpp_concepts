cmake_minimum_required(VERSION 3.15...4.0)

project(const_with_function_example)

set(exec_name const_with_function)
set(exec_source_file const_with_function.cpp)

find_package(spdlog REQUIRED)

message("Building example for ${exec_name}")
add_executable(${exec_name} ${exec_source_file})
target_link_libraries(${exec_name} PRIVATE spdlog::spdlog)
