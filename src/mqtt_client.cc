#include "mqtt_client.hpp"

#include <thread>
#include <iostream>
#include <cstring>

using namespace std::chrono_literals;

namespace mrobot {

namespace comm {

static void on_connect(mosquitto *mosq, void *obj, int rc) {
    (void)(mosq);

    auto *pClient = reinterpret_cast<MqttClient *>(obj);

    if (rc == 0) { // 0 == success
        std::cout << "on_connect: Connected to broker" << '\n';
    } else {
        std::cout << "on_connect: Connection to broker failed" << '\n';
        pClient->connect();
    }
}

static void on_disconnect(mosquitto *mosq, void *obj, int rc) {
    (void)(mosq);

    auto *pClient = reinterpret_cast<MqttClient *>(obj);

    if (rc == 0) { // 0 == success
        std::cout << "on_diconnect: Disconnected by client" << '\n';
    } else {
        std::cout << "on_connect: Disconnected from broker " << '\n';
        pClient->connect();
    }
}

static void on_message(struct mosquitto *mosq,
                      void *obj,
                      const struct mosquitto_message *msg) {
    (void)(mosq);

    std::cout << "Got message" << '\n';
    auto *pClient = reinterpret_cast<MqttClient *>(obj);
    pClient->receive(msg->topic, reinterpret_cast<unsigned char *>(msg->payload), msg->payloadlen);
}

bool MqttClient::initialize() {
    m_session = MqttSession::getSession();
    if (!m_session) {
        std::cout << "Error retreiving mosquitto session" << '\n';
        return false;
    }

    m_mosq = std::move(::mosquitto_new(m_clientName.c_str(), true, this));
    if (m_mosq == nullptr) {
        std::cout << "Error creating new mosquitto client" << '\n';
        return false;
    }

    mosquitto_connect_callback_set(m_mosq, on_connect);
    mosquitto_disconnect_callback_set(m_mosq, on_disconnect);
    mosquitto_message_callback_set(m_mosq, on_message);

    if (!connect()) {
        std::cout << "Failed connecting to broker, waiting for connection..." << '\n';
    }

    ::mosquitto_loop_start(m_mosq);

    return true;
}

void MqttClient::finalize() {
    disconnect();
    ::mosquitto_loop_stop(m_mosq, true);
    ::mosquitto_destroy(m_mosq);
}

bool MqttClient::connect() {
    bool res = false;
    int rc = 0;

    if (m_firstConnectionAttempt) {
        std::cout << "Trying to connect..." << '\n';
        rc = ::mosquitto_connect(m_mosq,
                                 m_brokerAddress.c_str(),
                                 m_brokerPort,
                                 s_keepAlive_sec);
        m_firstConnectionAttempt = false;
    } else {
        std::cout << "Trying to reconnect..." << '\n';
        rc = ::mosquitto_reconnect(m_mosq);
    }

    if (rc == MOSQ_ERR_SUCCESS) {
        std::cout << "Connected to broker" << '\n';
        res = true;
    } else if (rc == MOSQ_ERR_INVAL) {
        std::cout << "Invalid connection parameters" << '\n';
    } else if (rc == MOSQ_ERR_ERRNO) {
        std::cout << "Failed to connect to host: " << strerror(errno) << '\n';
    } else {
        std::cout << "Connect returned error: " << ::strerror(rc) << '\n';
    }

    return res;
}

bool MqttClient::disconnect() {
    ::mosquitto_disconnect(m_mosq);

    return true;
}

bool MqttClient::send(std::shared_ptr<std::vector<unsigned char>> payload, ::size_t size) {
    (void)(payload);
    (void)(size);

    // TODO: implement

    return true;
}

void MqttClient::receive(const char *topic, const unsigned char *payload, ::size_t size) {
    m_router->route(std::string(topic), payload, size);
}

} // namespace comm

} // namespace mrobot
