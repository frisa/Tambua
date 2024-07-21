cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(tambua-tools)
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_subdirectory(Libraries)
add_subdirectory(Applications)
