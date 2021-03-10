#include "message.hpp"

#include <iostream>
#include <cassert>

namespace mrobot {
    
namespace messaging {

template<uint32_t N>
void Router<N>::registerHandler(uint32_t messageId, Handler *handler)
{
    assert(messageId < N);

    m_handlers[messageId].push_back(handler);
}

template<uint32_t N>
void Router<N>::route(const unsigned char *payload, ::size_t size)
{
    if (payload == nullptr) {
        std::cout << "Routing error: Received null payload" << '\n';
        return;
    }

    const uint32_t messageId = *reinterpret_cast<const uint32_t *>(payload);
    if (size < sizeof(messageId)) {
        std::cout << "Routing error: Message too short for holding message id" << '\n';
        return;
    }

    if (messageId >= N) {
        std::cout << "Routing error: Invalid message ID " << std::to_string(messageId) << '\n';
        return;
    }

    auto messagePtr = m_factory->create(messageId, payload + sizeof(messageId), size - sizeof(messageId));
    if (!messagePtr) {
        std::cout << "Routing error: Invalid message" << '\n';
        return;
    }

    for (auto *handler : m_handlers[messageId]) {
        handler->handleMessage(messagePtr);
    }
}

} // namespace messaging

} // namespace mrobot
