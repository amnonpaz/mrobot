#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <vector>
#include <array>
#include <list>
#include <memory>

namespace mrobot {
    
namespace messaging {

class IncomingMessage {
    public:
        IncomingMessage() = default;
        virtual ~IncomingMessage() = default;

        virtual bool deserialize(const unsigned char *payload, ::size_t size) = 0;
};

class OutgoingMessage {
    public:
        OutgoingMessage() = default;
        virtual ~OutgoingMessage() = default;

        virtual const unsigned char *getPayload() const = 0;
        virtual ::size_t getSize() const = 0;
};

class Handler {
    public:
        Handler() = default;
        virtual ~Handler() = default;

        virtual void handleMessage(const std::unique_ptr<IncomingMessage> &message) = 0;
};

class IncomingRouter {
    public:
        explicit IncomingRouter(uint32_t maxMessageId) :
            m_handlers(static_cast<::size_t>(maxMessageId)) {}
        virtual ~IncomingRouter() = default;

        void registerHandler(uint32_t messageId, Handler *handler) const;
        bool route(uint32_t messageId, const unsigned char *payload, ::size_t size) const;

        uint32_t invalidMessageId() const { return m_handlers.size(); }

    protected:
        virtual std::unique_ptr<IncomingMessage> factory(uint32_t messageId) const = 0;

    private:
        mutable std::vector<std::list<Handler *>> m_handlers;
};

class Sender {
    public:
        Sender() = default;
        virtual ~Sender() = default;

        virtual bool send(uint32_t messageId, OutgoingMessage *pMessage) = 0;
};

} // namespace messaging

} // namespace mrobot

#endif // MESSAGE_H_
