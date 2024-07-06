cmake_minimum_required(VERSION 3.5)
project(tambua C CXX)

foreach(path ${CMAKE_PREFIX_PATH})
  message("Path: " ${path})
endforeach(path)

find_package(tensorflow-lite CONFIG)

message(STATUS "TensorFlow Lite library found at ${TENSORFLOW_LITE_DIR}")

add_executable(tambua ./src/program.cpp)
target_include_directories(tambua PRIVATE ${TENSORFLOW_LITE_DIR}/include)
target_link_libraries(tambua tensorflow-lite)
