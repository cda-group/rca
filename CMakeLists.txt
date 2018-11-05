cmake_minimum_required(VERSION 3.0)
project (rca)

add_subdirectory(cpu_features) 
add_executable(rca rca.c)
target_link_libraries(rca cpu_features)

