cmake_minimum_required(VERSION 3.14)
project(rtr)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic
        -I${CMAKE_SOURCE_DIR}/include/ -g -fno-stack-protector -z execstack)

include_directories ("${CMAKE_SOURCE_DIR}/src/")
include_directories ("${CMAKE_SOURCE_DIR}/include/")

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(rtr ${SOURCES})

target_link_libraries(rtr m)
