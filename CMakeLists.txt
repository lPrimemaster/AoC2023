cmake_minimum_required(VERSION 3.20)

project(aoc23 VERSION 1.0.0)

option(ENABLE_PPROF "Enables a naive performance profiler." OFF)

if(ENABLE_PPROF)
    add_definitions(-DENABLE_PPROF)
endif()

file(GLOB SRC_FILES src/*.cpp)

set(CMAKE_CXX_STANDARD 23)

foreach(file ${SRC_FILES})
    string(REPLACE "/" ";" file_sep ${file})
    list(GET file_sep -1 filename)
    message(STATUS ${filename})
    string(REGEX MATCH "day([0-9]+)\\.cpp" _ ${filename})
    add_executable("day${CMAKE_MATCH_1}" ${file})
    target_compile_options("day${CMAKE_MATCH_1}" PUBLIC -Wall -pedantic -Ofast)
endforeach()

