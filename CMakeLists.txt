cmake_minimum_required(VERSION 3.5)

project(libctmap)

option(ENABLE_TEST "enable test" OFF)

add_subdirectory(src)
if (ENABLE_TEST)
    add_subdirectory(test)
endif()
