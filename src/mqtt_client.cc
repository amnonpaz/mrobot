#include "mqtt_client.hpp"

namespace mrobot {

namespace comm {

bool MqttClient::initialize() {
    m_session = MqttSession::getSession();

    // TODO: implement

    return true;
}

void MqttClient::finalize() {
    // TODO: implement
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
