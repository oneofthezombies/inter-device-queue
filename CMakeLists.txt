cmake_minimum_required(VERSION 3.22)
project(lfwfspscq)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")

add_library(lfwfspscq STATIC src/lfwfspscq.c)

add_executable(lfwfspscq_test src/lfwfspscq_test.c)
target_link_libraries(lfwfspscq_test lfwfspscq)