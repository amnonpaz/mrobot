add_unittest(TEST gpio
             MAIN gpio.cc
             SOURCES gpio.cc
             LIBS stdc++fs
             DEFINES SYSTEM_GPIO_PATH_PREFIX="/tmp/mrobot/gpio_test/")

add_unittest(TEST conf
             MAIN conf.cc
             SOURCES conf.cc
             LIBS stdc++fs
             DEFINES DUMMY_CONF_PATH="/tmp/mrobot/conf/")
