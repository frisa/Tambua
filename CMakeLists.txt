cmake_minimum_required(VERSION 3.0)
project(tambua)

find_package(tensorflowlite REQUIRED)
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "TensorFlow Lite library: ${TENSORFLOWLITE_LIBRARIES}")
message(STATUS "TensorFlow Lite found: ${TENSORFLOWLITE_FOUND}")
message(STATUS "TensorFlow Lite include directory: ${TENSORFLOWLITE_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} src/program.cpp)
target_link_libraries(${PROJECT_NAME} tensorflow::tensorflowlite)