add_unittest(TEST mqtt
             MAIN mqtt.cc 
             SOURCES mqtt_client.cc message.cc
             LIBS pthread mosquitto)
