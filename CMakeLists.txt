cmake_minimum_required(VERSION 3.0)
project(tambua)

find_package(tensorflowlite REQUIRED)
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "TensorFlow Lite Includes: ${tensorflowlite_INCLUDE_DIR}")
message(STATUS "TensorFlow Lite Libraries: ${tensorflowlite_LIBRARIES}")

add_executable(${PROJECT_NAME} src/program.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${tensorflowlite_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} tensorflow::tensorflowlite)