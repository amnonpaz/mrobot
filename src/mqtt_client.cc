#include "mqtt_client.hpp"

#include <thread>
#include <iostream>
#include <cstring>

using namespace std::chrono_literals;

namespace mrobot {

namespace comm {

void ConnectionThread::work() {
    std::unique_lock<std::mutex> _lk(m_lock);

    while (m_running) {
        // if connect succeeds, we're waiting for on-connect result
        m_paused = connect();

        //auto now = std::chrono::system_clock::now();
        m_cv.wait_for(_lk, s_reconnectionInterval_ms*1ms,
                      [&]() { return !m_running; });

        if (!m_running) {
            std::cout << "Exiting connection loop" << '\n';
            break;
        }
    }

    disconnect();
}

bool ConnectionThread::connect() {
    int res = 0;

    if (m_firstConnectionAttempt) {
        std::cout << "Trying to connect..." << '\n';
        res = ::mosquitto_connect(m_owner,
                                  m_brokerAddress.c_str(),
                                  m_brokerPort,
                                  s_keepAlive_sec);
    } else {
        m_reconectionsCount++;
        std::cout << "Trying to reconnect [" << m_reconectionsCount << "]..." << '\n';
        res = ::mosquitto_reconnect(m_owner);
        m_firstConnectionAttempt = false;
    }

    if (res == MOSQ_ERR_INVAL) {
        std::cout << "Invalid connection parameters" << '\n';
    } else if (res == MOSQ_ERR_ERRNO) {
        std::cout << "Failed to connect to host: " << strerror(errno) << '\n';
    } else if (res == MOSQ_ERR_SUCCESS) {
        std::cout << "Connected to broker" << '\n';
    } else {
        std::cout << "Connect returned error: " << ::strerror(res) << '\n';
    }

    return res == MOSQ_ERR_SUCCESS;
}

void ConnectionThread::disconnect() {
    int rc = ::mosquitto_disconnect(m_owner);
    if (rc == MOSQ_ERR_SUCCESS) {
        std::cout << "Mosquitto client disconnected" << '\n';
    } else if (rc == MOSQ_ERR_NO_CONN) {
        std::cout << "Mosquitto client already diconnected" << '\n';
    }
}

void ConnectionThread::reconnect() {
    {
        std::unique_lock<std::mutex> _lk(m_lock);
        m_paused = false;
    }
    m_cv.notify_all();
}

bool ConnectionThread::start(::mosquitto *owner) {
    std::unique_lock<std::mutex> _lk(m_lock);
    if (m_running) {
        return true;
    }

    m_owner = owner;
    m_running = true;
    try {
        m_thread = std::thread(&ConnectionThread::work, this);
    } catch (...) {
        std::cout << "Failed creating connection thread" << '\n';
        m_running = false;
    }

    return m_running;
}

void ConnectionThread::stop() {
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

    auto *pClient = reinterpret_cast<MqttClient *>(obj);

    if (rc == 0) { // 0 == success
        std::cout << "Connected to broker" << '\n';
    } else {
        std::cout << "Connection to broker failed" << '\n';
        pClient->reconnect();
    }
}

static void onMessage(struct mosquitto *mosq,
                      void *obj,
                      const struct mosquitto_message *msg) {
    (void)(mosq);

    auto *pClient = reinterpret_cast<MqttClient *>(obj);
    pClient->receive(reinterpret_cast<unsigned char *>(msg->payload), msg->payloadlen);
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

    mosquitto_connect_callback_set(m_mosq, onConnect);
    mosquitto_message_callback_set(m_mosq, onMessage);

    return true;
}

void MqttClient::finalize() {
    disconnect();
    ::mosquitto_destroy(m_mosq);
}

bool MqttClient::connect() {
    if (!m_connector.start(m_mosq)) {
        std::cout << "Failed to start connection thread" << '\n';
        return false;
    }

    std::cout << "Connection thread started" << '\n';

    return true;
}

bool MqttClient::disconnect() {
    m_connector.stop();

    return true;
}

bool MqttClient::send(std::shared_ptr<std::vector<unsigned char>> payload, ::size_t size) {
    (void)(payload);
    (void)(size);

    // TODO: implement

    return true;
}

void MqttClient::receive(const unsigned char *payload, ::size_t size) {
    (void)(payload);
    (void)(size);
}

} // namespace comm

} // namespace mrobot
