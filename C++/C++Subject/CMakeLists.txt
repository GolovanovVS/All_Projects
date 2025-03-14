cmake_minimum_required(VERSION 3.22)
project(C__Contest)

set(CMAKE_CXX_STANDARD 20)

add_executable(biginteger biginteger.cpp biginteger.h)
add_executable(TaskAA TaskAA.cpp)
add_executable(TaskAAA TaskAAA.cpp)
add_executable(string string.cpp string.h)
add_executable(geometry geometry.cpp geometry.h)
add_executable(main main.cpp geometry.h)
add_executable(danger danger.cpp danger.h)
add_executable(deque deque.cpp deque.h)
add_executable(stackallocator stackallocator.cpp smart_pointers.h)
