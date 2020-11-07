cmake_minimum_required(VERSION 3.10)

#set the binary output to be the build directory, not the build/src directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR/..})

# set the project name
project(dgpen)

# add the src directory
add_subdirectory(src)

