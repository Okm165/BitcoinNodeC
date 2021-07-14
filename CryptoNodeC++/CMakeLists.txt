cmake_minimum_required(VERSION 3.13.0)
project(scripts VERSION 1.0.0)

execute_process(
    COMMAND ${CMAKE_COMMAND}
    -S ${CMAKE_SOURCE_DIR}/crypto
    -B ${CMAKE_BINARY_DIR}/crypto
    -G ${CMAKE_GENERATOR}
)
execute_process(
    COMMAND ${CMAKE_COMMAND}
    -S ${CMAKE_SOURCE_DIR}/primitives
    -B ${CMAKE_BINARY_DIR}/primitives
    -G ${CMAKE_GENERATOR}
)
execute_process(
    COMMAND ${CMAKE_COMMAND}
    -S ${CMAKE_SOURCE_DIR}/scripts
    -B ${CMAKE_BINARY_DIR}/scripts
    -G ${CMAKE_GENERATOR}
)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/crypto)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/primitives)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/scripts)

execute_process(COMMAND mkdir tmp WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
# execute_process(COMMAND mkdir include WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

set(REQ_LIBS ${CMAKE_SOURCE_DIR}/bin/require)

# unpack all static libs
execute_process(COMMAND ar -x ${CMAKE_BINARY_DIR}/crypto/libcrypto.a --output=${CMAKE_BINARY_DIR}/tmp)
message(STATUS "unpacking ${CMAKE_BINARY_DIR}/crypto/libcrypto.a")

execute_process(COMMAND ar -x ${CMAKE_BINARY_DIR}/primitives/libprimitives.a --output=${CMAKE_BINARY_DIR}/tmp)
message(STATUS "unpacking ${CMAKE_BINARY_DIR}/primitives/libprimitives.a")

execute_process(COMMAND ar -x ${CMAKE_BINARY_DIR}/scripts/libscripts.a --output=${CMAKE_BINARY_DIR}/tmp)
message(STATUS "unpacking ${CMAKE_BINARY_DIR}/scripts/libscripts.a")

execute_process(COMMAND ar -x ${REQ_LIBS}/libjsoncpp.a --output=${CMAKE_BINARY_DIR}/tmp)
message(STATUS "unpacking ${REQ_LIBS}/libjsoncpp.a")

execute_process(COMMAND ar -x ${REQ_LIBS}/libleveldb.a --output=${CMAKE_BINARY_DIR}/tmp)
message(STATUS "unpacking ${REQ_LIBS}/libleveldb.a")

configure_file(${REQ_LIBS}/libsecp256k1.o ${CMAKE_BINARY_DIR}/tmp COPYONLY)
message(STATUS "copying ${REQ_LIBS}/libsecp256k1.o")

message(STATUS "creating lib${PROJECT_NAME}.a static library")
FILE(GLOB SRCFILES ${CMAKE_BINARY_DIR}/tmp/*.o)
execute_process(COMMAND ar -crs ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.a ${SRCFILES})

message(STATUS "clearing ${CMAKE_BINARY_DIR}/tmp/")
execute_process(COMMAND rm -r  ${CMAKE_BINARY_DIR}/tmp/)

# message(STATUS "fetching header files")
# configure_file(${CMAKE_BINARY_DIR}/crypto/crypto.h ${CMAKE_BINARY_DIR}/include/crypto.h COPYONLY)
# configure_file(${CMAKE_BINARY_DIR}/primitives/primitives.h ${CMAKE_BINARY_DIR}/include/primitives.h COPYONLY)
# configure_file(${CMAKE_BINARY_DIR}/scripts/scripts.h ${CMAKE_BINARY_DIR}/include/scripts.h COPYONLY)

message(STATUS "replacing lib in ${CMAKE_SOURCE_DIR}/bin/libscripts.a")
configure_file(${CMAKE_BINARY_DIR}/libscripts.a ${CMAKE_SOURCE_DIR}/bin/libscripts.a COPYONLY)