#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <memory>

namespace mrobot {

namespace comm {

class Receiver {
    public:
        Receiver() = default;
        virtual ~Receiver() = default;

        virtual void receive(std::shared_ptr<char> payload, ::size_t size) = 0;
};

class Client {
    public:
        Client() = default;
        virtual ~Client() = default;

        virtual bool initialize() = 0;
        virtual void finalize() = 0;

        virtual bool connect() = 0;
        virtual bool disconnect() = 0;

        virtual void addReceiver(Receiver *receiver) = 0;

        virtual bool send(std::shared_ptr<char> payload, ::size_t size) = 0;
};

} // namespace comm

} // namespace mrobot

#endif // COMMUNICATION_H_
