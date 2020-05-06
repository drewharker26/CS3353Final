cmake_minimum_required(VERSION 3.10)
project(CS3353Final)
set (EXE_NAME BFTImplementation)
set(CMAKE_CXX_FLAGS "-Wall")

##########################################
# Students, edit the items below.  In the quotes,
# put the names of the input files as they should appear
# in the argv param to main.
# Example:
#   set (input01 "train_data.csv")
#   set (input02 "train_target.csv")
# etc....

set (input01 "testFile1.txt")
set (input02 "testFile2.txt")
set (input03 "")
set (input04 "")

foreach(file IN LISTS input01 input02 input03 input04)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 14)
add_executable(${EXE_NAME} main.cpp graph.h vertex.h handler.h handler.cpp)