cmake_minimum_required(VERSION 3.15...4.0)

project(virtual_class_example)

set(exec_name virtual_class)
set(exec_source_file virtual_class_example.cpp)

find_package(spdlog REQUIRED)

message("Building example for ${exec_name}")
add_executable(${exec_name} ${exec_source_file})
target_link_libraries(${exec_name} PRIVATE spdlog::spdlog)

add_executable(pure_virtual_class pure_virtual_class_example.cpp) 
target_link_libraries(pure_virtual_class PRIVATE spdlog::spdlog)
