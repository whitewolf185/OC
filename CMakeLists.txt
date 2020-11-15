cmake_minimum_required(VERSION 3.16)
project(OC C)

set(CMAKE_C_FLAGS "-Wall -Wformat=0")
set(CMAKE_C_STANDARD 90)

add_executable(OC "lab 2/main.c" "lab 2/vector.h")
add_executable(child "lab 2/child.c")

add_executable(lab3 "lab3/main.c")
set_source_files_properties("lab3/main.c" PROPERTIES COMPILE_FLAGS "-lpthread")


set_target_properties(
        child
        PROPERTIES
        SUFFIX ".out"
)