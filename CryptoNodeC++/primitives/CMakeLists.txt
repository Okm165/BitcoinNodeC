cmake_minimum_required(VERSION 3.11.0)
project(primitives VERSION 1.0.0)

add_library(${PROJECT_NAME}_target OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/bstream.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/fstream.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/leveldb.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/os.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/progressbar.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/util.cpp
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_library(${PROJECT_NAME}
    $<TARGET_OBJECTS:primitives_target>
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/primitives.h ${CMAKE_CURRENT_BINARY_DIR}/primitives.h COPYONLY)