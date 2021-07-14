cmake_minimum_required(VERSION 3.11.0)
project(scripts VERSION 1.0.0)

add_library(${PROJECT_NAME}_target OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/address.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/block.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/chain.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/index.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/rev.cpp
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_library(${PROJECT_NAME}
    $<TARGET_OBJECTS:scripts_target>
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts.h ${CMAKE_CURRENT_BINARY_DIR}/scripts.h COPYONLY)