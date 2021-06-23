set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)

include(${CMAKE_CURRENT_LIST_DIR}/prepend.cmake)
if (LINT)
    include(${CMAKE_CURRENT_LIST_DIR}/lint.cmake)
endif ()
include(${CMAKE_CURRENT_LIST_DIR}/tests.cmake)
