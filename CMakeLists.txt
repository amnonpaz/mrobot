cmake_minimum_required(VERSION 3.10.0)
project(mrobot
        VERSION 0.1.0
        DESCRIPTION "Robot For Maayan"
        LANGUAGES CXX)

option(LINT "Lint code" ON)
option(UNITTESTS "Build unittests" OFF)
option(NETWORKING_TESTS "Build networking tests" OFF)
option(SIMULATION "Build simulation" OFF)

include(cmake/env.cmake)

set(TESTING FALSE)
if(UNITTESTS OR NETWORKING_TESTS OR SIMULATION)
    set(TESTING TRUE)
endif()

if (NOT TESTING)
    add_subdirectory(src)
    add_subdirectory(scripts)
else()
    add_subdirectory(tests)
endif()
