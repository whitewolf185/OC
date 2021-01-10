cmake_minimum_required(VERSION 3.16)
project(lab6-8)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)


add_executable(server_fin server_fin.cpp)
target_link_libraries(server_fin zmqpp zmq sodium pthread)

add_executable(client client.cpp)
target_link_libraries(client zmqpp zmq sodium pthread)

add_dependencies(server_fin client)