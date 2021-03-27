set(TARGET_NAME robot_simulation)

set(SOURCES_PATH ${PROJECT_SOURCE_DIR}/src/)
set(MOCK_PATH ${PROJECT_SOURCE_DIR}/tests/mock/)

set(SOURCES
    ${SOURCES_PATH}/main.cc
    ${SOURCES_PATH}/robot.cc
    ${SOURCES_PATH}/conf.cc
    ${SOURCES_PATH}/led.cc
    ${SOURCES_PATH}/message.cc
    ${SOURCES_PATH}/mqtt_client.cc
    ${SOURCES_PATH}/robot_messages_router.cc
   )

set(MOCK
    ${MOCK_PATH}/gpio.cc 
   )

set(LIBS
    pthread
    mosquitto
   )

add_executable(${TARGET_NAME} ${SOURCES} ${MOCK})
target_compile_definitions(${TARGET_NAME} PRIVATE VERSION="SIMULATION")
target_include_directories(${TARGET_NAME} PRIVATE ${SOURCES_PATH})
target_link_libraries(${TARGET_NAME} ${LIBS})
