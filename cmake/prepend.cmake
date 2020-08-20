function(prepend)
    set(options )
    set(oneValueArgs DEST PREFIX)
    set(multiValueArgs VALUES)
    cmake_parse_arguments(PREPEND "${options}" "${oneValueArgs}"
                         "${multiValueArgs}" ${ARGN} )

    set(NEW_LIST "")
    foreach(V ${PREPEND_VALUES})
        list(APPEND NEW_LIST "${PREPEND_PREFIX}/${V}")
    endforeach()

    set(${PREPEND_DEST} "${NEW_LIST}" PARENT_SCOPE)
endfunction(prepend)
