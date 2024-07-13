cmake_minimum_required(VERSION 3.0)
project(tambua)

find_package(tensorflowlite REQUIRED)
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "TensorFlow Lite Includes: ${tensorflowlite_INCLUDE_DIR}")
message(STATUS "TensorFlow Lite Libraries: ${tensorflowlite_LIBRARIES}")

list(APPEND SRC_FILES 
    include/program.h
    src/program.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${tensorflowlite_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} tensorflow::tensorflowlite)