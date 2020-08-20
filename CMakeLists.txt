cmake_minimum_required(VERSION 3.10.0)
project(may_robot
        VERSION 0.1.0
        DESCRIPTION "Robot For Maayan"
        LANGUAGES CXX)

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-isystem)

add_subdirectory(src)
