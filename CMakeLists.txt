cmake_minimum_required(VERSION 3.10)
project(thinkgearLib)

set(CMAKE_C_STANDARD 11)

file(GLOB THINKGEAR_LIB
        thinkgearLib/common.*
        thinkgearLib/thinkgear.*
)
add_executable(thinkgearLib main.c ${THINKGEAR_LIB})

#target_link_libraries(thinkgearLib bluetooth)
