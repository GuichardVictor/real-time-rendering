cmake_minimum_required(VERSION 3.14)
project(rtr)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED COMPONENTS SDL2::SDL2)

add_compile_options(-Wall -Wextra -Werror -pedantic
        -I${CMAKE_SOURCE_DIR}/include/ -g -fno-stack-protector -z execstack)

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )

include_directories( ${Boost_INCLUDE_DIR} )
include_directories ("${CMAKE_SOURCE_DIR}/src/")
include_directories ("${CMAKE_SOURCE_DIR}/include/")
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(rtr ${SOURCES})

target_link_libraries(rtr ${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
