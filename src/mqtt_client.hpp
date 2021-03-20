#ifndef MOSQUITTO_CLIENT_H_
#define MOSQUITTO_CLIENT_H_

#include "comm.hpp"

#include <mosquitto.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

namespace mrobot {

namespace comm {

class MqttSession {
    public:
        MqttSession() {
            ::mosquitto_lib_init();

            int major, minor, revision;
            ::mosquitto_lib_version(&major, &minor, &revision);
            m_version = std::to_string(major) + "." + std::to_string(minor) +
                        std::to_string(revision);
        }

        ~MqttSession() {
            ::mosquitto_lib_cleanup();
        }

        const std::string &version() { return m_version; }

        static std::shared_ptr<MqttSession> getSession() {
            static std::shared_ptr<MqttSession> session{nullptr};
            if (!session) {
                session = std::make_shared<MqttSession>();
            }

            return session;
        }

    private:
        std::string m_version;
};


class ConnectionThread {
    public:
        ConnectionThread(std::string brokerAddress,
                         uint16_t brokerPort) :
            m_brokerAddress(std::move(brokerAddress)),
            m_brokerPort(brokerPort),
            m_owner(nullptr),
            m_running(false),
            m_paused(false),
            m_firstConnectionAttempt(true),
            m_reconectionsCount(0) {}
        ~ConnectionThread() = default;

        bool start(::mosquitto *owner);
        void stop();
        void reconnect();

    private:
        bool connect();
        void disconnect();
        void work();

        std::thread m_thread;
        std::mutex m_lock;
        std::condition_variable m_cv;

        const std::string m_brokerAddress;
        const uint16_t m_brokerPort;

        ::mosquitto *m_owner;
        bool m_running;
        bool m_paused;
        bool m_firstConnectionAttempt;
        uint32_t m_reconectionsCount;
        static constexpr uint32_t s_reconnectionInterval_ms = 30000;
        static constexpr uint32_t s_keepAlive_sec = 30;
};


class MqttClient final : public Client {
    public:
        MqttClient(std::string clientName,
                   std::string brokerAddress,
                   uint16_t brokerPort) :
            m_clientName(std::move(clientName)),
            m_connector(std::move(brokerAddress),
                        std::move(brokerPort)),
            m_session(nullptr) {}
        ~MqttClient() override = default;

        bool initialize() override ;
        void finalize() override ;

        bool connect() override ;
        bool disconnect() override ;
        inline void reconnect() { m_connector.reconnect(); }

        inline void addReceiver(Receiver *receiver) override {
            m_receivers.push_back(receiver);
        }

        bool send(std::shared_ptr<std::vector<unsigned char>> payload, ::size_t size) override ;
        void receive(const unsigned char *payload, ::size_t size);

    private:
        const std::string m_clientName;
        ConnectionThread m_connector;
        std::shared_ptr<MqttSession> m_session;
        ::mosquitto *m_mosq;

        std::list<Receiver *> m_receivers;
};

} // namespace comm

} // namespace mrobot

#endif // MOSQUITTO_CLIENT_H_
