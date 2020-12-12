cmake_minimum_required(VERSION 3.16)
project(lab5 C)
set(CMAKE_C_STANDARD 99)

add_executable(static static.c)
add_executable(dynamic dynamic.c)

add_library(lib1 SHARED function1.c)
set_target_properties(lib1 PROPERTIES
        PUBLIC_HEADER functions.h)

add_library(lib2 SHARED function2.c)
set_target_properties(lib2 PROPERTIES
        PUBLIC_HEADER functions.h)


target_link_libraries(lib1 m)
target_link_libraries(lib2 m)



set(custom_targets)

list(APPEND custom_targets lib1)
list(APPEND custom_targets lib2)

if(NOT lab_link_type)
    set(lab_link_type lib1)
endif()


function(invalid_target)
    message("Unknown target selected")
    message("All available targets:")
    foreach(target IN ITEMS ${custom_targets})
        message("    ${target}")
    endforeach()
endfunction()

if (${lab_link_type} IN_LIST custom_targets)
else()
    invalid_target()
    message( FATAL_ERROR "Invalid target selected" )
endif()


message("Link library for statMain will be now " ${lab_link_type})

target_link_libraries(static ${lab_link_type})
target_link_libraries(dynamic ${CMAKE_DL_LIBS})

add_dependencies(dynamic lib1 lib2)
