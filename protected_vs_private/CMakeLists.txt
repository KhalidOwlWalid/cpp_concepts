cmake_minimum_required(VERSION 3.15...4.0)

project(protected_vs_private)

set(exec_name access_specifier)
set(exec_source_file access_specifier_example.cpp)

find_package(spdlog REQUIRED)

message("Building example for ${exec_name}")
add_executable(${exec_name} ${exec_source_file})
target_link_libraries(${exec_name} PRIVATE spdlog::spdlog)
