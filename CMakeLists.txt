cmake_minimum_required(VERSION 3.16)
project(OC C)

#set(CMAKE_C_FLAGS "-lrt")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

set(CMAKE_C_STANDARD 99)

add_executable(lab4 "lab4/main.c" )
add_executable(child "lab4/child.c")
target_link_libraries(lab4 rt)
target_link_libraries(child rt)
target_link_libraries(child ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(lab4 ${CMAKE_THREAD_LIBS_INIT})


#add_executable(test_child "lab4/test_Anton_child.c")
#target_link_libraries(test_child rt)
#target_link_libraries(test_child ${CMAKE_THREAD_LIBS_INIT})

add_executable(lab3 "lab3/main.c" lab3/merge.h lab3/vector.h lab3/matfunc.h)
target_link_libraries(lab3 ${CMAKE_THREAD_LIBS_INIT})

#add_executable(lab3_test lab3/test_main.c)
#target_link_libraries(lab3_test ${CMAKE_THREAD_LIBS_INIT})
#set_source_files_properties("lab3/main.c" PROPERTIES COMPILE_FLAGS "-lpthread")


set_target_properties(
        child
        PROPERTIES
        SUFFIX ".out"
)

