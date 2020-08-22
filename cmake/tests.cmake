function(add_unittest)
    set(options )
    set(oneValueArgs TEST MAIN)
    set(multiValueArgs SOURCES INCLUDES LIBS DEFINES)
    cmake_parse_arguments(UNIT "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )

    set(SOURCES_PATH ${PROJECT_SOURCE_DIR}/src/)
    set(TEST_TARGET_NAME unit_${UNIT_TEST})

    prepend(DEST UNIT_FULL_PATH_SOURCES
            PREFIX ${SOURCES_PATH}
            VALUES ${UNIT_SOURCES})

    add_executable(${TEST_TARGET_NAME} ${UNIT_MAIN} ${UNIT_FULL_PATH_SOURCES})
    target_include_directories(${TEST_TARGET_NAME} PRIVATE
                               ${SOURCES_PATH} ${UNIT_INCLUDES})
    target_link_libraries(${TEST_TARGET_NAME} ${UNIT_LIBS})
    target_compile_definitions(${TEST_TARGET_NAME} PRIVATE TESTING ${UNIT_DEFINES})

endfunction(add_unittest)
