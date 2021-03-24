#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <vector>
#include <array>
#include <list>
#include <memory>

namespace mrobot {
    
namespace messaging {

class Message {
    public:
        Message() = default;
        virtual ~Message() = default;

        virtual bool deserialize(const unsigned char *payload, ::size_t size) = 0;

        // TODO: Implement serialize when needed
        // virtual void serialize(std::vector<unsigned char> &buffer) const = 0;
};


class Handler {
    public:
        Handler() = default;
        virtual ~Handler() = default;

        virtual void handleMessage(const std::unique_ptr<Message> &message) = 0;
};

class Router {
    public:
        Router(uint32_t maxMessageId) :
            m_handlers(static_cast<::size_t>(maxMessageId)) {}
        virtual ~Router() = default;

        void registerHandler(uint32_t messageId, Handler *handler);
        bool route(uint32_t messageId, const unsigned char *payload, ::size_t size) const;

        uint32_t invalidMessageId() const { return m_handlers.size(); }

    protected:
        virtual std::unique_ptr<Message> factory(uint32_t messageId) const = 0;

    private:
        std::vector<std::list<Handler *>> m_handlers;
};

} // namespace messaging

} // namespace mrobot

#endif // MESSAGE_H_
