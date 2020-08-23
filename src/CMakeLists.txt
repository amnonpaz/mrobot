set(TARGET_NAME ${PROJECT_NAME})
set(TARGET_VERSION ${PROJECT_VERSION})

set(SOURCES
    main.cc
    gpio.cc
   )

add_executable(${TARGET_NAME} ${SOURCES})
target_compile_definitions(${TARGET_NAME} PRIVATE VERSION="${TARGET_VERSION}")
