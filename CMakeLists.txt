cmake_minimum_required(VERSION 3.0)
project(tambua)
get_filename_component(TFL_INSTALL_DIR_ABS "~/TFLite" ABSOLUTE)
list(APPEND CMAKE_PREFIX_PATH "${TFL_INSTALL_DIR_ABS}")
find_package(tensorflow-lite CONFIG)

add_executable(${PROJECT_NAME} program.cpp)
target_link_directories(${PROJECT_NAME} PRIVATE ${TFL_INSTALL_DIR_ABS}/lib)
target_link_libraries(${PROJECT_NAME} tensorflow-lite)