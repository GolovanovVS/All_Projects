cmake_minimum_required(VERSION 3.27)
project(AlgContest_7)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-g -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")


add_executable(main main.cpp)
add_executable(TaskA TaskA.cpp)
add_executable(TaskB TaskB.cpp)
add_executable(TaskC TaskC.cpp)
add_executable(TaskD TaskD.cpp)
add_executable(TaskE TaskE.cpp)
add_executable(TaskG TaskG.cpp)
add_executable(TaskF TaskF.cpp)
add_executable(TaskH TaskH.cpp)
add_executable(TaskI TaskI.cpp)
add_executable(TaskJ TaskJ.cpp)
