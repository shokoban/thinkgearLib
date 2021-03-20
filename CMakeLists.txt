cmake_minimum_required(VERSION 3.10)
project(thinkgearLib)

set(CMAKE_CXX_STANDARD 14)

file(GLOB THINKGEAR_LIB
        thinkgearLib/common.*
        thinkgearLib/thinkgear.*
)
add_executable(thinkgearLib main.cpp ${THINKGEAR_LIB})

#target_link_libraries(thinkgearLib bluetooth)
