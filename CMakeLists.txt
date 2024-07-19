cmake_minimum_required(VERSION 3.5)
project(tambua)

# Get the dependencies
find_package(tensorflowlite REQUIRED)
find_package(absl REQUIRED)

# Add application source files
list(APPEND SRC_FILES 
    src/program.cpp
    src/helpers.cpp
)

# Add delegate source files
set(TF_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tensorflow_repository/lite)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/delegates/default_execution_provider.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/delegates/delegate_provider.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/tool_params.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/command_line_flags.cc)
list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/evaluation/utils.cc)

# Add delegate for XNNPACK
set(TFLITE_ENABLE_XNNPACK ON CACHE BOOL "Enable XNNPACK delegate")
if(TFLITE_ENABLE_XNNPACK)
    list(APPEND SRC_FILES ${TF_SOURCE_DIR}/tools/delegates/xnnpack_delegate_provider.cc)

else()
    set(CC_OPTIONS "-DTFLITE_WITHOUT_XNNPACK")
endif()


# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add options
target_compile_options(${PROJECT_NAME} PRIVATE ${CC_OPTIONS})

# Link libraries
target_link_libraries(${PROJECT_NAME} tensorflow::tensorflowlite)
target_link_libraries(${PROJECT_NAME} abseil::abseil)