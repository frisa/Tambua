cmake_minimum_required(VERSION 3.5)
find_package(tensorflow-lite CONFIG)
project(tambua)
add_executable(${PROJECT_NAME} src/program.cpp)
