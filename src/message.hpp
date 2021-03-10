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


class Factory {
    public:
        Factory() = default;
        ~Factory() = default;

        virtual std::unique_ptr<Message> create(uint32_t messageId,
                                                const unsigned char *payload,
                                                ::size_t size) const = 0;
};

class Handler {
    public:
        Handler() = default;
        virtual ~Handler() = default;

        virtual void handleMessage(const std::unique_ptr<Message> &message) = 0;
};

class Router {
    public:
        Router(uint32_t maxMessageId, const Factory *factory) :
            m_factory(factory),
            m_handlers(static_cast<::size_t>(maxMessageId)) {}
        virtual ~Router() = default;

        void registerHandler(uint32_t messageId, Handler *handler);
        bool route(uint32_t messageId, const unsigned char *payload, ::size_t size);

    private:
        const Factory *m_factory;
        std::vector<std::list<Handler *>> m_handlers;
};

} // namespace messaging

} // namespace mrobot
