set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-isystem)

function(lint)
    set(options )
    set(oneValueArgs )
    set(multiValueArgs FILES)
    cmake_parse_arguments(LINT "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )

    prepend(DEST LINT_FULL_PATH_FILES
            PREFIX ${CMAKE_CURRENT_SOURCE_DIR}
            VALUES ${LINT_FILES})

    execute_process(COMMAND clang-format -style=mozilla ${LINT_FULL_PATH_FILES})
endfunction(lint)
