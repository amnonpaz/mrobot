#include "mqtt_client.hpp"

#include <thread>
#include <iostream>
#include <cstring>

using namespace std::chrono_literals;

namespace mrobot {

namespace comm {

void MqttClient::ReconnectionThread::work() {
    while (true) {
        {
            std::unique_lock<std::mutex> _lk(m_lock);
            m_cv.wait_for(_lk, s_reconnectionInterval_ms*1ms,
                    [&]() { return m_running && !m_paused; });

            if (!m_running) {
                std::cout << "Exiting reconnection loop" << '\n';
                break;
            }
        }

        std::cout << "Trying to reconnect..." << '\n';
        int rc = ::mosquitto_reconnect(m_owner);
        if (rc == MOSQ_ERR_SUCCESS) {
            m_paused = true;
        }
    }
}

void MqttClient::ReconnectionThread::reconnect() {
    {
        std::unique_lock<std::mutex> _lk(m_lock);
        m_paused = false;
    }
    m_cv.notify_all();
}

bool MqttClient::ReconnectionThread::start(::mosquitto *owner) {
    std::unique_lock<std::mutex> _lk(m_lock);
    if (m_running) {
        return true;
    }

    m_owner = owner;
    m_running = true;
    try {
        m_thread = std::thread(&ReconnectionThread::work, this);
    } catch (...) {
        std::cout << "Failed creating reconnection thread" << '\n';
        m_running = false;
    }

    return m_running;
}

void MqttClient::ReconnectionThread::stop() {
    {
        std::unique_lock<std::mutex> _lk(m_lock);
        if (!m_running) {
            return;
        }

        m_running = false;
    }

    m_cv.notify_all();
    m_thread.join();
}


static void onConnect(mosquitto *mosq, void *obj, int rc) {
    (void)(mosq);

    auto pClient = reinterpret_cast<MqttClient *>(obj);

    if (rc == 0) { // 0 == success
        std::cout << "Connected to broker" << '\n';
    } else {
        std::cout << "Connection to broker failed" << '\n';
        pClient->reconnect();
    }
}

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

    mosquitto_connect_callback_set(m_mosq, onConnect);

    return true;
}

void MqttClient::finalize() {
    ::mosquitto_destroy(m_mosq);
}

bool MqttClient::connect() {
    if (!m_reconnector.start(m_mosq)) {
        return false;
    }

    int res = ::mosquitto_connect(m_mosq,
                                  m_brokerAddress.c_str(),
                                  m_brokerPort,
                                  s_keepAlive_sec);
    if (res == MOSQ_ERR_INVAL) {
        std::cout << "Invalid connection parameters" << '\n';
    } else if (res == MOSQ_ERR_ERRNO) {
        std::cout << "Failed to connect to host: " << strerror(errno) << '\n';
    } else if (res == MOSQ_ERR_SUCCESS) {
        std::cout << "Connected to host, waiting for broker connection..." << '\n';
    }

    return res == MOSQ_ERR_SUCCESS;
}

bool MqttClient::disconnect() {
    int rc = ::mosquitto_disconnect(m_mosq);
    if (rc == MOSQ_ERR_SUCCESS) {
        std::cout << "Mosquitto client disconnected" << '\n';
        m_reconnector.stop();
    } else if (rc == MOSQ_ERR_NO_CONN) {
        std::cout << "Mosquitto client already diconnected" << '\n';
    }

    return (rc == MOSQ_ERR_SUCCESS);
}

bool MqttClient::send(std::shared_ptr<char> payload, ::size_t size) {
    (void)(payload);
    (void)(size);

    // TODO: implement

    return true;
}

} // namespace comm

} // namespace mrobot
