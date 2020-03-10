cmake_minimum_required(VERSION 3.14)
project(rtr)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED COMPONENTS SDL2::SDL2)

add_compile_options(-Wall -Wextra -Werror -pedantic
        -I${CMAKE_SOURCE_DIR}/include/ -g -fno-stack-protector -z execstack)

include_directories ("${CMAKE_SOURCE_DIR}/src/")
include_directories ("${CMAKE_SOURCE_DIR}/include/")

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(rtr ${SOURCES})

include_directories(rtr ${SDL2_INCLUDE_DIRS})
target_link_libraries(rtr ${SDL2_LIBRARIES} SDL2::SDL2)
