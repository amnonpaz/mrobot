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

    switch (rc) {
        case MOSQ_ERR_SUCCESS:
            std::cout << "Connected to broker" << '\n';
            res = subscribe();
            if (!res) {
                std::cout << "Subscription failed" << '\n';
                disconnect();
            }
            break;
        case MOSQ_ERR_INVAL:
            std::cout << "Invalid connection parameters" << '\n';
            break;
        case MOSQ_ERR_ERRNO:
            std::cout << "Failed to connect to host: " << strerror(errno) << '\n';
            break;
        default:
            std::cout << "Connect returned error: " << ::strerror(rc) << '\n';
            break;
    }

    return res;
}

bool MqttClient::disconnect() {
    ::mosquitto_disconnect(m_mosq);

    return true;
}

bool MqttClient::subscribe() {
    bool res = true;

    for (uint32_t messageId = 0; messageId != m_router->invalidMessageId(); messageId++) {
        const std::string &topic = m_router->getTopic(messageId);
        int rc = mosquitto_subscribe(m_mosq, nullptr, topic.c_str(), s_defaultQOS);
        switch (rc) {
            case MOSQ_ERR_SUCCESS:
                std::cout << "Subscribed to " << topic << '\n';
                break;
            case MOSQ_ERR_INVAL:
                std::cout << "Subscribe error: Invalid parameters" << '\n';
                break;
            case MOSQ_ERR_NOMEM:
                std::cout << "Subscribe error: Not enought memory" << '\n';
                break;
            case MOSQ_ERR_NO_CONN:
                std::cout << "Subscribe error: Client isn't connected to broker" << '\n';
                break;
            case MOSQ_ERR_MALFORMED_UTF8:
                std::cout << "Subscribe error: Topic is not a valid UTF-8" << '\n';
                break;
            case MOSQ_ERR_OVERSIZE_PACKET:
                std::cout << "Subscribe error: Packet size not supported by broker" << '\n';
                break;
            default:
                std::cout << "Subscribe error: Unknown error" << '\n';
                break;
        }

        if (rc != MOSQ_ERR_SUCCESS) {
            res = false;
            break;
        }
    }

    return res;
}

bool MqttClient::send(const char *topic, const unsigned char *payload, ::size_t size) {
    bool res = false;

    int rc = mosquitto_publish(m_mosq, nullptr, topic, size, payload, s_defaultQOS, true);
    switch (rc) {
        case MOSQ_ERR_SUCCESS:
            res = true;
            break;
        case MOSQ_ERR_INVAL:
            std::cout << "Publish error: Invalid parameters" << '\n';
            break;
        case MOSQ_ERR_NOMEM:
            std::cout << "Publish error: Not enought memory" << '\n';
            break;
        case MOSQ_ERR_NO_CONN:
            std::cout << "Publish error: Client isn't connected to broker" << '\n';
            break;
        case MOSQ_ERR_PROTOCOL:
            std::cout << "Publish error: Protocol error" << '\n';
            break;
        case MOSQ_ERR_PAYLOAD_SIZE:
            std::cout << "Publish error: Payload too large" << '\n';
            break;
        case MOSQ_ERR_MALFORMED_UTF8:
            std::cout << "Publish error: Topic is not a valid UTF-8" << '\n';
            break;
        case MOSQ_ERR_QOS_NOT_SUPPORTED:
            std::cout << "Publish error: QoS not supported" << '\n';
            break;
        case MOSQ_ERR_OVERSIZE_PACKET:
            std::cout << "Publish error: Packet size not supported by broker" << '\n';
            break;
        default:
            std::cout << "Publish error: Unknown error" << '\n';
            break;
    }

    return res;
}

void MqttClient::receive(const char *topic, const unsigned char *payload, ::size_t size) {
    m_router->route(std::string(topic), payload, size);
}

} // namespace comm

} // namespace mrobot
