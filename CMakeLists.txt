cmake_minimum_required(VERSION 3.15)
project(TSP)

set (input01 "inputFile01")
set (input02 "inputFile02")
set (input03 "inputFile03")

foreach(file IN LISTS input01 input02 input03)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 14)

add_executable(TSP main.cpp Graph.h NN.h Set.h Driver.h Driver.cpp Chris.h)