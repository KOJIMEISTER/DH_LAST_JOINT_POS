cmake_minimum_required(VERSION 3.14)

project(dha)

include_directories(".")

set(SRC_LIST DH.cpp main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall")
add_executable(${PROJECT_NAME} ${SRC_LIST})
