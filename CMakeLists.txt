cmake_minimum_required(VERSION 3.5)
project(tambua)

find_package(tensorflowlite REQUIRED)

# Add application source files
list(APPEND SRC_FILES 
    include/program.h
    include/bmp.h
    include/log.h
    src/program.cpp
    src/bmp.cpp
    src/log.cpp
)

# Add XNNPACK delegate source files
set(TF_SOURCE_DIR "/home/jan/Repositories/tensorflow/tensorflow/lite")
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/delegates/xnnpack_delegate_provider.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/delegates/delegate_provider.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/tool_params.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/command_line_flags.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/evaluation/utils.cc)

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} tensorflow::tensorflowlite)