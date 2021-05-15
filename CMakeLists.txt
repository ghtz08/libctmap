cmake_minimum_required(VERSION 3.5)

project(libctmap)

option(BUILD_TEST "build test" OFF)

add_subdirectory(src)
if (BUILD_TEST)
    add_subdirectory(test)
endif()
