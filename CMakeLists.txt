cmake_minimum_required(VERSION 2.8)

project(VirtuosoMisc)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x  ")
  
add_executable(MiscTest tests/test1.cpp)
add_executable(ModTest tests/modtest.cpp) 
