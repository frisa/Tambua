cmake_minimum_required(VERSION 3.10)
set(EXECUTABLE_NAME "tambua-qt-classifier")
project(${EXECUTABLE_NAME})

# Setup paths
IF(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "~/Qt/6.7.2/macos")
ELSE()
    list(APPEND CMAKE_PREFIX_PATH "~/Qt/6.7.2/gcc_64")
ENDIF()

# Find packages
find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Quick)

# Setup variables
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(PROJECT_SOURCES
    src/main.cpp
    src/logic.cpp
    resources.qrc
    )

set(PROJECT_INCLUDES
    include/logic.h
    )
# Add the Qt executable
qt_standard_project_setup(REQUIRES 6.5)
qt6_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_INCLUDES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
    )
