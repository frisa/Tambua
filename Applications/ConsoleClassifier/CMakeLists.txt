cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(tambua-console)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} tambua-classifier)
