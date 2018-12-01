cmake_minimum_required(VERSION 3.0)
project(rca)


set(_HDRS
  include/rca.h
)

add_library(rca
    ${_HDRS}
    src/rca.c
)

set(CMAKE_SYSTEM_NAME Linux)

# Library
add_subdirectory(cpu_features) 
target_link_libraries(rca cpu_features -static)

# Test Bin
add_executable(test_bin  src/bin/test_bin.c)
target_link_libraries(test_bin PRIVATE rca)



