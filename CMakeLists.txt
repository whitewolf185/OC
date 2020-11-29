cmake_minimum_required(VERSION 3.16)
project(OC C)

set(CMAKE_C_FLAGS "-O3")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

set(CMAKE_C_STANDARD 99)

add_executable(OC "lab 2/main.c" "lab 2/vector.h")
add_executable(child "lab 2/child.c")

add_executable(lab3 "lab3/main.c" lab3/merge.h lab3/vector.h lab3/matfunc.h)
target_link_libraries(lab3 ${CMAKE_THREAD_LIBS_INIT})

add_executable(lab3_test lab3/test_main.c)
target_link_libraries(lab3_test ${CMAKE_THREAD_LIBS_INIT})
#set_source_files_properties("lab3/main.c" PROPERTIES COMPILE_FLAGS "-lpthread")


set_target_properties(
        child
        PROPERTIES
        SUFFIX ".out"
)

add_executable(lab4_test "lab4/u.c")
set_target_properties(
        lab4_test
        PROPERTIES
        SUFFIX ".out"
)