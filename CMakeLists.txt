cmake_minimum_required(VERSION 3.0)
project(rca)

set(CMAKE_SYSTEM_NAME Linux)
add_subdirectory(cpu_features) 

# Bin
add_executable(rca src/rca.c)
target_link_libraries(rca cpu_features -static)

