set(TARGET_NAME ${PROJECT_NAME})
set(TARGET_VERSION ${PROJECT_VERSION})

set(SOURCES
    main.cc
    robot.cc
    gpio.cc
    conf.cc
    led.cc
    message.cc
    mqtt_client.cc
   )

set(LIBS
    pthread
    mosquitto
   )

add_executable(${TARGET_NAME} ${SOURCES})
target_compile_definitions(${TARGET_NAME} PRIVATE VERSION="${TARGET_VERSION}")
target_link_libraries(${TARGET_NAME} ${LIBS})

install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin/)
