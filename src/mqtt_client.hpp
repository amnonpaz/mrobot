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

class MqttClient final : public Client {
    public:
        MqttClient(std::string clientName,
                   std::string brokerAddress,
                   uint16_t brokerPort) :
            m_clientName(std::move(clientName)),
            m_brokerAddress(std::move(brokerAddress)),
            m_brokerPort(brokerPort),
            m_session(nullptr) {}
        ~MqttClient() override = default;

        bool initialize() override ;
        void finalize() override ;

        bool connect() override ;
        bool disconnect() override ;
        inline void reconnect() { m_reconnector.reconnect(); }

        inline void addReceiver(Receiver *receiver) override {
            m_receivers.push_back(receiver);
        }

        bool send(std::shared_ptr<std::vector<unsigned char>> payload, ::size_t size) override ;
        void receive(const unsigned char *payload, ::size_t size);

    private:
        class ReconnectionThread {
            public:
                ReconnectionThread() :
                    m_owner(nullptr),
                    m_running(false),
                    m_paused(true) {}
                ~ReconnectionThread() = default;

                bool start(::mosquitto *owner);
                void stop();
                void reconnect();

            private:
                void work();

                std::thread m_thread;
                std::mutex m_lock;
                std::condition_variable m_cv;

                ::mosquitto *m_owner;
                bool m_running;
                bool m_paused;
                static constexpr uint32_t s_reconnectionInterval_ms = 2000;
        };

        const std::string m_clientName;
        const std::string m_brokerAddress;
        const uint16_t m_brokerPort;
        std::shared_ptr<MqttSession> m_session;
        ::mosquitto *m_mosq;

        std::list<Receiver *> m_receivers;

        ReconnectionThread m_reconnector;
        static const uint32_t s_keepAlive_sec = 30;
};

} // namespace comm

} // namespace mrobot

#endif // MOSQUITTO_CLIENT_H_
