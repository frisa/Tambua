cmake_minimum_required(VERSION 3.0)
project(tambua)
# Include the Conan toolchain file
message (STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR}/ ${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)

find_package(tensorflowlite REQUIRED)

add_executable(${PROJECT_NAME} src/program.cpp)
target_link_libraries(${PROJECT_NAME} tensorflow::tensorflowlite)