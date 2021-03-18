add_unittest(TEST mqtt
             MAIN mqtt.cc
             SOURCES mqtt_client.cc
             LIBS pthread mosquitto)
