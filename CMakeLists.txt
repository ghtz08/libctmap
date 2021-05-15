cmake_minimum_required(VERSION 3.5)

project(libctmap)

option(ENABLE_TEST "enable test" OFF)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
if (ENABLE_TEST)
    add_subdirectory(test)
endif()
