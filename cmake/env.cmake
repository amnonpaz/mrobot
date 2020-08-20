set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)

include(${CMAKE_CURRENT_LIST_DIR}/prepend.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/lint.cmake)
