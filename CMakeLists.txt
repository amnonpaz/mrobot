cmake_minimum_required(VERSION 3.10.0)
project(may_robot
        VERSION 0.1.0
        DESCRIPTION "Robot For Maayan"
        LANGUAGES CXX)

option(UNITTESTS "Build unittests" OFF)

include(cmake/env.cmake)

set(TESTING FALSE)
if(UNITTESTS)
    set(TESTING TRUE)
endif()

if (NOT TESTING)
    add_subdirectory(src)
else()
    add_subdirectory(tests)
endif()
