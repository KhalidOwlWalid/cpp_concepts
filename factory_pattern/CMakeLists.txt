cmake_minimum_required(VERSION 3.15...4.0)

project(const_with_function_example)

find_package(spdlog REQUIRED)

add_executable(simple_factory_pattern simple_factory_pattern.cpp)
target_link_libraries(simple_factory_pattern PRIVATE spdlog::spdlog)

add_executable(advanced_factory_pattern advanced_factory_pattern.cpp)
target_link_libraries(advanced_factory_pattern PRIVATE spdlog::spdlog)
