cmake_minimum_required(VERSION 3.5)

project(tambua C CXX)
find_package(tensorflow-lite CONFIG)

message(STATUS "TensorFlow Lite library found at ${TENSORFLOW_LITE_DIR}")
add_executable(tambua ./src/program.cpp)
target_link_libraries(tambua tensorflow-lite)
