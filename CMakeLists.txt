cmake_minimum_required(VERSION 3.0)
project(tambua)
find_package(tensorflow-lite REQUIRED)

add_executable(${PROJECT_NAME} program.cpp)
target_link_libraries(${PROJECT_NAME} tensorflow-lite)