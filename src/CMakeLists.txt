include_directories(${SOCKPIPE_SOURCE_DIR}/include)

file(GLOB SRCS ${SOCKPIPE_SOURCE_DIR}/src/*.c)

add_executable(sockpipe ${SRCS})
target_link_libraries(sockpipe uv)
