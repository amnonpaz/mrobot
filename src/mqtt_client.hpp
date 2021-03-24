#ifndef MOSQUITTO_CLIENT_H_
#define MOSQUITTO_CLIENT_H_

#include "message.hpp"

#include <mosquitto.h>

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

class MqttRouter : public messaging::Router {
    public:
        MqttRouter(uint32_t maxMessagesTypes) :
            Router(maxMessagesTypes) {}
        virtual ~MqttRouter() = default;

        inline bool route(const std::string &topic, const unsigned char *payload, ::size_t size) const {
            return Router::route(topicToMessageId(topic), payload, size);
        }

        inline const std::string &getTopic(uint32_t messageId) const {
            return messageIdToTopic(messageId);
        }

    protected:
        virtual uint32_t topicToMessageId(const std::string &topic) const = 0;
        virtual const std::string &messageIdToTopic(uint32_t messageId) const = 0;
};


class MqttClient final {
    public:
        MqttClient(std::string clientName,
                   std::string brokerAddress,
                   uint16_t brokerPort,
                   const MqttRouter *router) :
            m_clientName(std::move(clientName)),
            m_brokerAddress(std::move(brokerAddress)),
            m_brokerPort(brokerPort),
            m_firstConnectionAttempt(true),
            m_session(nullptr),
            m_router(router) {}

        bool initialize();
        void finalize();

        bool connect();
        bool disconnect();

        bool send(const char *topic, const unsigned char *payload, ::size_t size);
        void receive(const char *topic,
                     const unsigned char *payload,
                     ::size_t size);

    private:
        bool subscribe() ;

        const std::string m_clientName;
        const std::string m_brokerAddress;
        const uint16_t m_brokerPort;
        bool m_firstConnectionAttempt;
        std::shared_ptr<MqttSession> m_session;
        ::mosquitto *m_mosq;

        const MqttRouter *m_router;

        static constexpr uint32_t s_keepAlive_sec = 30;
        static constexpr int s_defaultQOS = 1;
};

} // namespace comm

} // namespace mrobot

#endif // MOSQUITTO_CLIENT_H_
