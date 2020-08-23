add_unittest(TEST gpio
             MAIN gpio.cc
             SOURCES gpio.cc
             LIBS stdc++fs
             DEFINES SYSTEM_GPIO_PATH_PREFIX="/tmp/gpio_test/")
