#include "mqtt_client.hpp"

#include <iostream>

namespace mrobot {

namespace comm {

bool MqttClient::initialize() {
    m_session = MqttSession::getSession();
    if (!m_session) {
        std::cout << "Error retreiving mosquitto session" << '\n';
        return false;
    }

    m_mosq = ::mosquitto_new(m_clientName.c_str(), true, this);
    if (m_mosq == nullptr) {
        std::cout << "Error creating new mosquitto client" << '\n';
        return false;
    }

    return true;
}

void MqttClient::finalize() {
    ::mosquitto_destroy(m_mosq);
}

bool MqttClient::connect() {
    // TODO: implement

    return true;
}

bool MqttClient::disconnect() {
    // TODO: implement

    return true;
}

bool MqttClient::send(std::shared_ptr<char> payload, ::size_t size) {
    (void)(payload);
    (void)(size);

    // TODO: implement

    return true;
}

} // namespace comm

} // namespace mrobot
