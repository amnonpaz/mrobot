#ifndef MOSQUITTO_CLIENT_H_
#define MOSQUITTO_CLIENT_H_

#include "comm.hpp"

#include <list>

namespace mrobot {

namespace comm {

class MosquittoClient final : public Client {
    public:
        MosquittoClient(std::string clientName,
                        std::string brokerAddress,
                        uint16_t brokerPort) :
            m_clientName(std::move(clientName)),
            m_brokerAddress(std::move(brokerAddress)),
            m_brokerPort(brokerPort) {}
        ~MosquittoClient() override = default;

        bool initialize() override ;
        void finalize() override ;

        bool connect() override ;
        bool disconnect() override ;

        inline void addReceiver(Receiver *receiver) override {
            m_receivers.push_back(receiver);
        }

        bool send(std::shared_ptr<char> payload, ::size_t size) override ;

    private:
        const std::string m_clientName;
        const std::string m_brokerAddress;
        const uint16_t m_brokerPort;

        std::list<Receiver *> m_receivers;
        
};

} // namespace comm

} // namespace mrobot

#endif // MOSQUITTO_CLIENT_H_
