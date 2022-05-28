if (UNITTESTS)
    add_subdirectory(unit)
endif()

if (NETWORKING_TESTS)
    add_subdirectory(networking)
endif()

if (BLUETOOTH_TESTS)
    add_subdirectory(bluetooth)
endif()

if (SIMULATION)
    add_subdirectory(simulation)
endif()
