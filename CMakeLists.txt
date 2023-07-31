cmake_minimum_required(VERSION 3.10)

project(thinkgearLib)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE THINKGEAR_LIB
        thinkgearLib/*.c
        thinkgearLib/utils/*.c
        thinkgearLib/core/*.c
        thinkgearLib/core/win/*.c
        thinkgearLib/core/linux/*.c
)
add_executable(thinkgearLib main.c ${THINKGEAR_LIB})
if(UNIX)
target_link_libraries(thinkgearLib bluetooth)
endif()
