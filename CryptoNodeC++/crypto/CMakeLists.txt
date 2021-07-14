cmake_minimum_required(VERSION 3.11.0)
project(crypto VERSION 1.0.0)

add_library(${PROJECT_NAME}_target OBJECT
            ${CMAKE_CURRENT_SOURCE_DIR}/ripemd160/ripemd160.c
            ${CMAKE_CURRENT_SOURCE_DIR}/sha256/hl_sha256.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sha256/hl_sha256wrapper.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/base58.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/bech32.cpp
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_library(${PROJECT_NAME}
    $<TARGET_OBJECTS:crypto_target>
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/crypto.h ${CMAKE_CURRENT_BINARY_DIR}/crypto.h COPYONLY)