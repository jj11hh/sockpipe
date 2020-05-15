cmake_minimum_required (VERSION 3.10.0)
project (SOCKPIPE)

add_compile_options("-std=gnu11" "-Wall")

if(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_BUILD_TYPE Debug)

if(UNIX)
    add_compile_options("-D LOG_USE_COLOR")
endif(UNIX)

set (CMAKE_BINARY_DIR ${SOCKPIPE_SOURCE_DIR}/build/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory (src)
