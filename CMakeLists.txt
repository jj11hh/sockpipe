cmake_minimum_required (VERSION 3.10.0)
project (SOCKPIPE)

add_compile_options("-std=gnu11")
if(UNIX)
   add_compile_options("-D LOG_USE_COLOR")
endif(UNIX)

add_subdirectory (src)
